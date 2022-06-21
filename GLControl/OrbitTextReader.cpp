#include "pch.h"

#include "OrbitTextReader.h"
#include "Interpolator.h"

#include <thread>
//#include <mutex>

namespace orbit
{
	static std::string file_to_orbit(const std::string& sFile_)
	{
		size_t nPointPos = sFile_.find(".");
		size_t nNamePos = sFile_.find_last_of("\\") + 2;

		return sFile_.substr(nNamePos, nPointPos - nNamePos);
	}

	static std::vector<Snpt> readFile(const char* sFileName_, bool bAllRecord_ = true)
	{
		std::ifstream strFile(sFileName_);
		if (!strFile.is_open())
			return std::vector<Snpt>();

		unsigned nRecCount;
		strFile >> nRecCount;

		std::vector<Snpt> vNpt(nRecCount);

		for (unsigned i = 0; i < nRecCount; ++i)
		{
			strFile >> vNpt[i].nSpectrumNumb >> vNpt[i].nInterferogramID;
			strFile >> vNpt[i].fJulianDate >> vNpt[i].sUTC;
			strFile >> vNpt[i].fDistancToSun >> vNpt[i].fLongitude >> vNpt[i].fLatitude >> vNpt[i].fLS >> vNpt[i].fLocalTime >> vNpt[i].fSunZenit >> vNpt[i].fObserverZenit;
			strFile >> vNpt[i].fSurfaceTemp >> vNpt[i].fDustOpticalDepth >> vNpt[i].fIceOpticalDepth;
			strFile >> vNpt[i].nLevelCount;

			if (!bAllRecord_)
				break;

			vNpt[i].vLevel.resize(vNpt[i].nLevelCount);

			for (unsigned j = 0; j < vNpt[i].nLevelCount; ++j)
				strFile >> vNpt[i].vLevel[j].fAltitude >> vNpt[i].vLevel[j].fPresure >> vNpt[i].vLevel[j].fTempereture;
		}

		strFile.close();
		return vNpt;
	}

	//static std::mutex  :mutex g_i_mutex;

	static void threadWork(std::vector<unsigned>& result, const std::string& sFile_, float fLatitude_, float fLongitude_)
	{
		std::vector<Snpt> vNpt = readFile(sFile_.c_str());

		for (int j = 0; j < vNpt.size(); ++j)
		{
			if (std::abs(vNpt[j].fLatitude - fLatitude_) < 1 && (std::abs(vNpt[j].fLongitude - fLongitude_) < 1))
			{
				//const std::lock_guard<std::mutex> lock(g_i_mutex);
				result.push_back(std::stoi(file_to_orbit(sFile_)));
				break;
			}
		}
	}

	//--------------------------------------------------------------------------

	//-------------------------------------------------------------------------------

	OrbitTextReader::OrbitTextReader()
	{
	}

	OrbitTextReader::~OrbitTextReader()
	{
	}

	bool OrbitTextReader::init()
	{
		std::string sOrbitDir;
		if (!lib::XMLreader::getSting(lib::XMLreader::getNode(getConfig(), OrbitDir()), sOrbitDir))
		{
			toLog("ERROR! Can't find orbit dir tag in config");
			return false;
		}

		if (!lib::XMLreader::getInt(lib::XMLreader::getNode(getConfig(), TemperartureAltitudeMax()), m_nAltitudeMAX))
			m_nAltitudeMAX = 60;

		if (!lib::XMLreader::getInt(lib::XMLreader::getNode(getConfig(), TemperatureInterpolateCount()), m_nInterpolateCount))
			m_nInterpolateCount = 30;

		m_vFileList = lib::create_file_list(sOrbitDir.c_str());

		for (int i = 0; i < m_vFileList.size(); ++i)
		{
			std::string  sNumber = file_to_orbit(m_vFileList[i]);

			try
			{
				int nNumber = std::stoi(sNumber);
				m_mOrbit[nNumber] = i;
			}
			catch (const std::exception&)
			{
				toLog("ERROR! Can't convert to int: " + sNumber);
				m_vFileList.erase(m_vFileList.begin() + i);
				--i;
				continue;
			}

			//-----------------------------------------------

			std::vector<Snpt> vNpt = readFile(m_vFileList[i].c_str(), false);

			if (!vNpt.empty())
				m_mLS[int(vNpt[0].fLS * 100)] = i;
			else
			{
				toLog("ERROR! Can't open npt file: " + m_vFileList[i]);
				m_vFileList.erase(m_vFileList.begin() + i);
				--i;
				continue;
			}
		}

		return true;
	}

	void OrbitTextReader::setFileIndex(unsigned nFirstIndex_, unsigned nLastIndex_, std::vector<SPairLevel>& vLevelData_, bool bClearLevel_)
	{
		if (bClearLevel_)
			vLevelData_.clear();

		for (unsigned f = nFirstIndex_; f < nLastIndex_ && f < m_vFileList.size(); ++f)
		{
			std::vector<Snpt> vNpt = readFile(m_vFileList[f].c_str());

			if (f == nFirstIndex_)
				m_Snpt = vNpt[0];

			for (unsigned i = 0; i < vNpt.size() - 1; ++i)
			{
				if (std::abs(vNpt[i + 0].fLatitude - vNpt[i + 1].fLatitude) > m_fAngleContinues || std::abs(vNpt[i + 0].fLongitude - vNpt[i + 1].fLongitude > m_fAngleContinues))
					continue;

				Interpolator interpolator1(vNpt[i + 0].vLevel.data(), vNpt[i + 0].nLevelCount);
				float fAltitudeMin1 = interpolator1.getAltitudeMin();
				float fAltitudeMax1 = interpolator1.getAltitudeMax();

				Interpolator interpolator2(vNpt[i + 1].vLevel.data(), vNpt[i + 1].nLevelCount);
				float fAltitudeMin2 = interpolator2.getAltitudeMin();
				float fAltitudeMax2 = interpolator2.getAltitudeMax();

				float fAltitudeMinMax = std::max<float>(fAltitudeMin1, fAltitudeMin2);
				float fAltitudeMaxMin = std::min<float>(std::min<float>(fAltitudeMax1, fAltitudeMax2), (float)m_nAltitudeMAX);

				float fAltitudeStep = (fAltitudeMaxMin - fAltitudeMinMax) / m_nInterpolateCount;

				SPairLevel vertex;
				vertex.fLatitude_begin = glm::radians(vNpt[i + 0].fLatitude);
				vertex.fLatitude_end = glm::radians(vNpt[i + 1].fLatitude);
				vertex.fLongitude_begin = glm::radians(vNpt[i + 0].fLongitude);
				vertex.fLongitude_end = glm::radians(vNpt[i + 1].fLongitude);
				vertex.fDistane_begin = vNpt[i + 0].vLevel[0].fAltitude * 1000;
				vertex.fDistane_end = vNpt[i + 1].vLevel[0].fAltitude * 1000;

				vertex.fAltitudeMinMax = fAltitudeMinMax * 1000;
				vertex.fAltitudeStep = fAltitudeStep * 1000;

				//-----------------------------------------------------------------------------

				std::vector<float> vTemperature1;
				std::vector<float> vTemperature2;

				interpolator1.getTemperature(fAltitudeMinMax, fAltitudeMaxMin, m_nInterpolateCount, vTemperature1);
				interpolator2.getTemperature(fAltitudeMinMax, fAltitudeMaxMin, m_nInterpolateCount, vTemperature2);

				vertex.vTemperature.resize(vTemperature1.size() + vTemperature2.size());

				for (int k = 0; k < vTemperature1.size(); ++k)
				{
					vertex.vTemperature[2 * k + 0] = vTemperature1[k];
					vertex.vTemperature[2 * k + 1] = vTemperature2[k];
				}

				//-----------------------------------------------------------------------------

				vLevelData_.push_back(std::move(vertex));
			}
		}
	}

	std::vector<Snpt> OrbitTextReader::getNpt(const char* sFileName_, bool bAllRecord_, bool bIncludeLevels_)
	{
		return readFile(sFileName_, true);
	}

	size_t OrbitTextReader::getRecCount(unsigned nIndex_)
	{
		return m_vvNpt[nIndex_].size();
	}

	size_t OrbitTextReader::getFileCount()
	{
		return m_vFileList.size();
	}

	size_t OrbitTextReader::getCount()
	{
		return m_vvNpt.size();
	}

	std::vector<unsigned> OrbitTextReader::getOrbitListByCoord(float fLatitude_, float fLongitude_)
	{
		std::vector<unsigned> result;

		//std::thread* vThread = new(std::nothrow) std::thread[m_vFileList.size()];

		for (int i = 0; i < m_vFileList.size(); ++i)
		{
			std::vector<Snpt> vNpt = readFile(m_vFileList[i].c_str(), true);
			//vThread[i] = std::thread(threadWork, result, m_vFileList[i], fLatitude_, fLongitude_);

			for (int j = 0; j < vNpt.size(); ++j)
			{
				if (std::abs(vNpt[j].fLatitude - fLatitude_) < 1 && (std::abs(vNpt[j].fLongitude - fLongitude_) < 1))
				{
					result.push_back(std::stoi(file_to_orbit(m_vFileList[i])));
					break;
				}
			}

		}

		//for (unsigned i = 0; i < m_vFileList.size(); ++i)
		//	vThread[i].join();

		//delete[] vThread;

		return std::move(result);
	}

	unsigned OrbitTextReader::getSpectrumNumb()
	{
		return m_Snpt.nSpectrumNumb;
	}

	float OrbitTextReader::getLS()
	{
		return m_Snpt.fLS;
	}

	float OrbitTextReader::getJulianDate()
	{
		return m_Snpt.fJulianDate;
	}

	float OrbitTextReader::getLocalTime()
	{
		return m_Snpt.fLocalTime;
	}

	unsigned OrbitTextReader::getOrbit_by_number(unsigned nNumber_)
	{
		if (m_mOrbit.find(nNumber_) != m_mOrbit.end())
			return m_mOrbit[nNumber_];

		for (auto const& [key, value] : m_mOrbit)
			if (key >= (int)nNumber_)
				return value;

		return UINT_MAX;
	}

	unsigned OrbitTextReader::getOrbit_by_LS(unsigned nNumber_)
	{
		if (m_mLS.find(nNumber_) != m_mLS.end())
			return m_mLS[nNumber_];

		for (auto const& [key, value] : m_mLS)
			if (key >= (int)nNumber_)
				return value;

		return UINT_MAX;
	}

	std::string OrbitTextReader::getUTC()
	{
		return m_Snpt.sUTC;
	}
}