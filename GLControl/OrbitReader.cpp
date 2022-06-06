#include "pch.h"

#include "OrbitReader.h"
#include "Interpolator.h"

#include "Lib.h"

#include <fstream>

namespace orbit
{
	std::vector<Snpt> readFile(const char* sFileName_)
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
			return false;

		if (!lib::XMLreader::getInt(lib::XMLreader::getNode(getConfig(), TemperartureAltitudeMax()), m_nAltitudeMAX))
			m_nAltitudeMAX = 60;

		if (!lib::XMLreader::getInt(lib::XMLreader::getNode(getConfig(), TemperatureInterpolateCount()), m_nInterpolateCount))
			m_nInterpolateCount = 30;

		m_vFileList = lib::create_file_list(sOrbitDir.c_str());

		bool bError = false;

		return !bError;
	}

	void OrbitReader::setFileIndex(unsigned nIndex_, std::vector<SPairLevel>& vLevelData_)
	{
		m_nFileId = nIndex_;

		std::vector<Snpt> vNpt = readFile(m_vFileList[m_nFileId].c_str());
		vLevelData_.clear();

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

			float fAltitudeMinMax = std::max(fAltitudeMin1, fAltitudeMin2);
			float fAltitudeMaxMin = std::min(std::min(fAltitudeMax1, fAltitudeMax2), (float)m_nAltitudeMAX);

			float fAltitudeStep = (fAltitudeMaxMin - fAltitudeMinMax) / m_nInterpolateCount;

			std::vector<float> vTemperature1;
			std::vector<float> vTemperature2;

			interpolator1.getTemperature(fAltitudeMinMax, fAltitudeMaxMin, m_nInterpolateCount, vTemperature1);
			interpolator2.getTemperature(fAltitudeMinMax, fAltitudeMaxMin, m_nInterpolateCount, vTemperature2);

			SPairLevel vertex;
			vertex.fLatitude_begin	= glm::radians(vNpt[i + 0].fLatitude);
			vertex.fLatitude_end	= glm::radians(vNpt[i + 1].fLatitude);
			vertex.fLongitude_begin = glm::radians(vNpt[i + 0].fLongitude);
			vertex.fLongitude_end	= glm::radians(vNpt[i + 1].fLongitude);
			vertex.fDistane_begin	= vNpt[i + 0].vLevel[0].fAltitude * 1000;
			vertex.fDistane_end		= vNpt[i + 1].vLevel[0].fAltitude * 1000;

			vertex.fAltitudeMinMax	= fAltitudeMinMax * 1000;
			vertex.fAltitudeStep	= fAltitudeStep * 1000;

			vertex.vTemperature.resize(vTemperature1.size() + vTemperature2.size());
			for (int k = 0; k < vTemperature1.size(); ++k)
			{
				vertex.vTemperature[2 * k + 0] = vTemperature1[k];
				vertex.vTemperature[2 * k + 1] = vTemperature2[k];
			}

			vLevelData_.push_back(std::move(vertex));
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

	std::vector<Snpt> OrbitReader::operator[](unsigned nIndex_)
	{
		return std::move(readFile(m_vFileList[nIndex_].c_str()));
	}
}