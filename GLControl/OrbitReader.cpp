#include "pch.h"

#include "OrbitReader.h"
#include "Interpolator.h"

namespace orbit
{
	std::vector<Snpt> readFile(const char* sFileName_, bool bHoleFile_ = true)
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

			if (!bHoleFile_)
				break;

			vNpt[i].vLevel.resize(vNpt[i].nLevelCount);
			for (unsigned j = 0; j < vNpt[i].nLevelCount; ++j)
				strFile >> vNpt[i].vLevel[j].fAltitude >> vNpt[i].vLevel[j].fPresure >> vNpt[i].vLevel[j].fTempereture;
		}

		strFile.close();
		return vNpt;
	}

	//-------------------------------------------------------------------------------

	OrbitReader::OrbitReader()
	{
	}

	OrbitReader::~OrbitReader()
	{
	}

	bool OrbitReader::init()
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
			size_t nPointPos = m_vFileList[i].find(".");
			size_t nNamePos = m_vFileList[i].find_last_of("\\") + 2;

			std::string  sNumber = m_vFileList[i].substr(nNamePos, nPointPos - nNamePos);

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

	void OrbitReader::setFileIndex(unsigned nFirstIndex_, unsigned nLastIndex_, std::vector<SPairLevel>& vLevelData_)
	{
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

	size_t OrbitReader::getRecCount(unsigned nIndex_)
	{
		return m_vvNpt[nIndex_].size();
	}

	size_t OrbitReader::getFileCount()
	{
		return m_vFileList.size();
	}

	size_t OrbitReader::getCount()
	{
		return m_vvNpt.size();
	}
	unsigned OrbitReader::getSpectrumNumb()
	{
		return m_Snpt.nSpectrumNumb;
	}

	float OrbitReader::getLS()
	{
		return m_Snpt.fLS;
	}

	float OrbitReader::getJulianDate()
	{
		return m_Snpt.fJulianDate;
	}

	float OrbitReader::getLocalTime()
	{
		return m_Snpt.fLocalTime;
	}

	unsigned OrbitReader::getOrbit_by_number(unsigned nNumber_)
	{
		if (m_mOrbit.find(nNumber_) != m_mOrbit.end())
			return m_mOrbit[nNumber_];

		for (auto const& [key, value] : m_mOrbit)
			if (key >= (int)nNumber_)
				return value;

		return UINT_MAX;
	}

	unsigned OrbitReader::getOrbit_by_LS(unsigned nNumber_)
	{
		if (m_mLS.find(nNumber_) != m_mLS.end())
			return m_mLS[nNumber_];

		for (auto const& [key, value] : m_mLS)
			if (key >= (int)nNumber_)
				return value;

		return UINT_MAX;
	}

	std::string OrbitReader::getUTC()
	{
		return m_Snpt.sUTC;
	}
}