#include "pch.h"

#include "OrbitTextReader.h"
#include "Interpolator.h"

#pragma managed(push, off)

#include <thread>

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
		std::vector<char> tempBuffer(3 * 1024 * 1024);

		std::ifstream strFile(sFileName_);

		if (!strFile.is_open())
			return std::vector<Snpt>();

		strFile.rdbuf()->pubsetbuf(tempBuffer.data(), tempBuffer.size());
		unsigned nOrbit = std::stoi(file_to_orbit(sFileName_));

		unsigned nRecCount;
		strFile >> nRecCount;

		std::vector<Snpt> vNpt(nRecCount);

		for (unsigned i = 0; i < nRecCount; ++i)
		{
			vNpt[i].nOrbit = nOrbit;
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
		return std::move(vNpt);
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
		//std::string sOrbitDir;
		//if (!lib::XMLreader::getSting(lib::XMLreader::getNode(getConfig(), OrbitDir()), sOrbitDir))
		//{
		//	toLog("ERROR! Can't find orbit dir tag in config");
		//	return false;
		//}

		//if (!lib::XMLreader::getInt(lib::XMLreader::getNode(getConfig(), TemperartureAltitudeMax()), m_nAltitudeMAX))
		//	m_nAltitudeMAX = 60;

		//if (!lib::XMLreader::getInt(lib::XMLreader::getNode(getConfig(), TemperatureInterpolateCount()), m_nInterpolateCount))
		//	m_nInterpolateCount = 30;

		//m_vFileList = lib::create_file_list(sOrbitDir.c_str());

		//for (int i = 0; i < m_vFileList.size(); ++i)
		//{
		//	std::string  sNumber = file_to_orbit(m_vFileList[i]);

		//	try
		//	{
		//		int nNumber = std::stoi(sNumber);
		//		m_mOrbit[nNumber] = i;
		//	}
		//	catch (const std::exception&)
		//	{
		//		toLog("ERROR! Can't convert to int: " + sNumber);
		//		m_vFileList.erase(m_vFileList.begin() + i);
		//		--i;
		//		continue;
		//	}

		//	//-----------------------------------------------

		//	std::vector<Snpt> vNpt = readFile(m_vFileList[i].c_str(), false);

		//	if (!vNpt.empty())
		//		m_mLS[int(vNpt[0].fLS * 100)] = i;
		//	else
		//	{
		//		toLog("ERROR! Can't open npt file: " + m_vFileList[i]);
		//		m_vFileList.erase(m_vFileList.begin() + i);
		//		--i;
		//		continue;
		//	}
		//}

		return true;
	}

	void OrbitTextReader::setFileIndex(unsigned nFirstIndex_, unsigned nLastIndex_, std::vector<SPairLevel>& vLevelData_, bool bClearLevel_)
	{
	}

	std::vector<Snpt> OrbitTextReader::getNpt(const char* sFileName_, bool bAllRecord_, bool bIncludeLevels_)
	{
		return std::move(readFile(sFileName_, true));
	}

	size_t OrbitTextReader::getOrbitCount()
	{
		return m_vFileList.size();
	}
}

#pragma managed(pop)