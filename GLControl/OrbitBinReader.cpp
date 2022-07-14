#include "pch.h"
#include "OrbitBinReader.h"
#include "Interpolator.h"

#pragma managed(push, off)

namespace orbit
{
    OrbitBinReader::OrbitBinReader()
	{
	}

	OrbitBinReader::~OrbitBinReader()
	{
	}

	bool OrbitBinReader::init()
	{
        std::string sLevelFile;
        if (!lib::XMLreader::getSting(lib::XMLreader::getNode(getConfig(), Key::LevelFileName()), sLevelFile))
            sLevelFile = "Level.bin";

        if (fopen_s(&m_pLevelFile, sLevelFile.c_str(), "rb") != 0)
        {
            toLog("Can't open Levels bin file: " + sLevelFile);
            return false;
        }

        _fseeki64(m_pLevelFile, 0, SEEK_END);
        long nLevelFileSize = ftell(m_pLevelFile);
        _fseeki64(m_pLevelFile, 0, SEEK_SET);

        unsigned nLevelBufferSize;
        if (!lib::XMLreader::getInt(lib::XMLreader::getNode(getConfig(), Key::LevelBufferSize()), nLevelBufferSize))
            nLevelBufferSize = 10;

        unsigned nAskBufferSize = std::min<unsigned>(std::min<unsigned>((unsigned)nLevelFileSize, 1024 * 1024 * nLevelBufferSize), INT_MAX);

        try
        {
            m_vLevelReadBuffer.reserve(nAskBufferSize);
            m_vLevelReadBuffer.resize(nAskBufferSize);
        }
        catch (const std::exception&)
        {
            toLog("ERROR Can't setup buffer for levels bin file. Asked for Mb: " + std::to_string(nAskBufferSize));
            nAskBufferSize /= 10;
        }

        try
        {
            if (m_vLevelReadBuffer.empty())
            {
                toLog("Ask for new size Mb: " + std::to_string(nAskBufferSize));
                m_vLevelReadBuffer.reserve(nAskBufferSize);
                m_vLevelReadBuffer.resize(nAskBufferSize);
            }
        }
        catch (const std::exception&)
        {
            toLog("ERROR Can't setup buffer for levels bin file. Asked for Mb: " + std::to_string(nLevelBufferSize));
        }

        if (!m_vLevelReadBuffer.empty())
            if (setvbuf(m_pLevelFile, m_vLevelReadBuffer.data(), _IOFBF, m_vLevelReadBuffer.size()) != 0)
            {
                toLog("ERROR Can't setup buffer for levels bin file.");
            }

       //--------------------------------------------------------------------------------------------

        std::string sOrbitFile;
        if (!lib::XMLreader::getSting(lib::XMLreader::getNode(getConfig(), Key::OrbitFileName()), sOrbitFile))
            sOrbitFile = "Orbit.bin";

        FILE* pOrbitFile;
        if (fopen_s(&pOrbitFile, sOrbitFile.c_str(), "rb") != 0)
        {
            toLog("Can't open Orbit bin file: " + sOrbitFile);
            return false;
        }

        _fseeki64(pOrbitFile, 0, SEEK_END);
        long nOrbitFileSize = ftell(pOrbitFile);
        _fseeki64(pOrbitFile, 0, SEEK_SET);

        m_vOrbit.resize(nOrbitFileSize / sizeof(OrbitFile));

        if (fread(m_vOrbit.data(), nOrbitFileSize, 1, pOrbitFile) != 1)
        {
            toLog("Can't read from Orbit bin file: " + sOrbitFile);
            return false;
        }

        fclose(pOrbitFile);

        //--------------------------------------------------------------------------------------------

        std::string sNptFile;
        if (!lib::XMLreader::getSting(lib::XMLreader::getNode(getConfig(), Key::NptFileName()), sNptFile))
            sNptFile = "Npt.bin";

        FILE* pNptFile;
        if (fopen_s(&pNptFile, sNptFile.c_str(), "rb") != 0)
        {
            toLog("Can't open Npt bin file: " + sNptFile);
            return false;
        }

        _fseeki64(pNptFile, 0, SEEK_END);
        long nONptFileSize = ftell(pNptFile);
        _fseeki64(pNptFile, 0, SEEK_SET);

        m_vNpt.resize(nONptFileSize / sizeof(NptFile));

        if (fread(&m_vNpt[0], nONptFileSize, 1, pNptFile) != 1)
        {
            toLog("Can't read from Npt bin file: " + sNptFile);
            return false;
        }

        fclose(pNptFile);

        //--------------------------------------------------------------------------------------------

        for (int i = 0; i < m_vOrbit.size(); ++i)
        {
            m_mOrbit[m_vOrbit[i].nOrbit] = i;

            //-----------------------------------------------

            std::vector<Snpt> vNpt = get_vNpt(m_vOrbit[i], false);

            if (!vNpt.empty())
                m_mLS[int(vNpt[0].fLS * 100)] = i;
            else
            {
                toLog("bin files cracked. ");
                return false;
            }
        }

        //--------------------------------------------------------------------------------------------

        lib::XMLreader::getInt(lib::XMLreader::getNode(getConfig(), Key::TemperatureInterpolateCount()), m_nInterpolateCount);

        if (m_nInterpolateCount == 0)
            m_nInterpolateCount = 1;

        //--------------------------------------------------------------------------------------------

        return true;
	}

    std::vector<Snpt> OrbitBinReader::get_vNpt(const OrbitFile& orbit_, bool bAllRecord_ = true)
    {
        std::vector<Snpt> vNpt;

        unsigned nBegin = orbit_.nBegin / sizeof(NptFile);
        unsigned nEnd = orbit_.nEnd / sizeof(NptFile);


        vNpt.resize(nEnd - nBegin - 1);
        for (int i = 0; i < vNpt.size(); ++i)
        {
            vNpt[i].nSpectrumNumb = m_vNpt[i + nBegin].nSpectrumNumb;
            vNpt[i].nInterferogramID = m_vNpt[i + nBegin].nInterferogramID;
            vNpt[i].fJulianDate = m_vNpt[i + nBegin].fJulianDate;
            vNpt[i].sUTC = std::string(m_vNpt[i + nBegin].sUTC);
            vNpt[i].fDistancToSun = m_vNpt[i + nBegin].fDistancToSun;
            vNpt[i].fLongitude = m_vNpt[i + nBegin].fLongitude;
            vNpt[i].fLatitude = m_vNpt[i + nBegin].fLatitude;
            vNpt[i].fLS = m_vNpt[i + nBegin].fLS;
            vNpt[i].fLocalTime = m_vNpt[i + nBegin].fLocalTime;
            vNpt[i].fSunZenit = m_vNpt[i + nBegin].fSunZenit;
            vNpt[i].fObserverZenit = m_vNpt[i + nBegin].fObserverZenit;
            vNpt[i].fSurfaceTemp = m_vNpt[i + nBegin].fSurfaceTemp;
            vNpt[i].fDustOpticalDepth = m_vNpt[i + nBegin].fDustOpticalDepth;
            vNpt[i].fIceOpticalDepth = m_vNpt[i + nBegin].fIceOpticalDepth;
            vNpt[i].nLevelCount = m_vNpt[i + nBegin].nLevelCount;

            if (!bAllRecord_)
                break;

            vNpt[i].vLevel.resize(vNpt[i].nLevelCount);

            _fseeki64(m_pLevelFile, m_vNpt[i + nBegin].nBegin, SEEK_SET);

            if (fread(vNpt[i].vLevel.data(), vNpt[i].nLevelCount * sizeof(SLevel), 1, m_pLevelFile) != 1)
            {
                toLog("ERROR get_vNpt fread ");
                break;
            }
        }

        return std::vector<Snpt>(vNpt);
    }

    void OrbitBinReader::setFileIndex(unsigned nFirstIndex_, std::vector<SPairLevel>& vLevelData_)
    {
        vLevelData_.clear();

        unsigned nIndex = std::min<unsigned>(nFirstIndex_, (unsigned)m_vOrbit.size());

        std::vector<Snpt> vNpt = get_vNpt(m_vOrbit[nIndex]);
 
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
            vertex.fDistance_begin = vNpt[i + 0].vLevel[0].fAltitude * 1000;
 
            vertex.fDistance_end = vNpt[i + 1].vLevel[0].fAltitude * 1000;
 
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

    size_t OrbitBinReader::getOrbitCount()
    {
        return m_vOrbit.size();
    }

    std::vector<unsigned> OrbitBinReader::getOrbitListByCoord(float fLatitude_, float fLongitude_)
    {
        std::vector<unsigned> result;

#pragma omp parallel for
            for (int i = 0; i < m_vOrbit.size(); ++i)
            {
                std::vector<Snpt> vNpt = get_vNpt(m_vOrbit[i]);

                for (int j = 0; j < vNpt.size(); ++j)
                    if (std::abs(vNpt[j].fLatitude - fLatitude_) < 1 && (std::abs(vNpt[j].fLongitude - fLongitude_) < 1))
                    {
                        result.push_back(m_vOrbit[i].nOrbit);
                        break;
                    }
            }

        return std::move(result);
    }

    Snpt OrbitBinReader::getNpt()
    {
        return m_Snpt;
    }

    unsigned OrbitBinReader::getOrbitNumber_by_OrbitIndex(unsigned nIndex_)
    {
        for (auto const& [key, value] : m_mOrbit)
            if (value >= (int)nIndex_)
                return key;

        return UINT_MAX;
    }

    unsigned OrbitBinReader::getOrbitIndex_by_OrbitNumber(unsigned nNumber_)
    {
        if (m_mOrbit.find(nNumber_) != m_mOrbit.end())
            return m_mOrbit[nNumber_];

        for (auto const& [key, value] : m_mOrbit)
            if (key >= (int)nNumber_)
                return value;

        return UINT_MAX;
    }

    unsigned OrbitBinReader::getOrbit_by_LS(unsigned nNumber_)
    {
        if (m_mLS.find(nNumber_) != m_mLS.end())
            return m_mLS[nNumber_];

        for (auto const& [key, value] : m_mLS)
            if (key >= (int)nNumber_)
                return value;

        return UINT_MAX;
    }
}

#pragma managed(pop)