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
        if (!lib::XMLreader::getSting(lib::XMLreader::getNode(getConfig(), LevelFileName()), sLevelFile))
            sLevelFile = ".\\Level.bin";

        if (fopen_s(&m_pLevelFile, sLevelFile.c_str(), "rb") != 0)
            return false;

       //--------------------------------------------------------------------------------------------

        std::string sOrbitFile;
        if (!lib::XMLreader::getSting(lib::XMLreader::getNode(getConfig(), OrbitFileName()), sOrbitFile))
            sOrbitFile = ".\\Orbit.bin";

        FILE* pOrbitFile;
        if (fopen_s(&pOrbitFile, sOrbitFile.c_str(), "rb") != 0)
            return false;

        _fseeki64(pOrbitFile, 0, SEEK_END);
        long nOrbitFileSize = ftell(pOrbitFile);
        _fseeki64(pOrbitFile, 0, SEEK_SET);

        m_vOrbit.resize(nOrbitFileSize / sizeof(OrbitFile));

        if (fread(m_vOrbit.data(), nOrbitFileSize, 1, pOrbitFile) != 1)
            return false;

        fclose(pOrbitFile);

        //--------------------------------------------------------------------------------------------

        std::string sNptFile;
        if (!lib::XMLreader::getSting(lib::XMLreader::getNode(getConfig(), NptFileName()), sNptFile))
            sNptFile = ".\\Npt.bin";

        FILE* pNptFile;
        if (fopen_s(&pNptFile, sNptFile.c_str(), "rb") != 0)
            return false;

        _fseeki64(pNptFile, 0, SEEK_END);
        long nONptFileSize = ftell(pNptFile);
        _fseeki64(pNptFile, 0, SEEK_SET);

        m_vNpt.resize(nONptFileSize / sizeof(NptFile));

        if (fread(&m_vNpt[0], nONptFileSize, 1, pNptFile) != 1)
            return false;

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
            //vNpt[i].sUTC = m_vNpt[i + orbit.nBegin].sUTC;
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

            toLog("nLevelCount: " + std::to_string(vNpt[i].nLevelCount));


            vNpt[i].vLevel.resize(vNpt[i].nLevelCount);

            _fseeki64(m_pLevelFile, m_vNpt[i + nBegin].nBegin, SEEK_SET);

            if (fread(vNpt[i].vLevel.data(), vNpt[i].nLevelCount * sizeof(SLevel), 1, m_pLevelFile) != 1)
            {
                toLog("break: ");
                break;
            }
        }

        return std::vector<Snpt>(vNpt);
    }

    void OrbitBinReader::setFileIndex(unsigned nFirstIndex_, unsigned nLastIndex_, std::vector<SPairLevel>& vLevelData_, bool bClearLevel_)
    {
        if (bClearLevel_)
            vLevelData_.clear();

        for (unsigned f = nFirstIndex_; f < nLastIndex_ && f < m_vOrbit.size(); ++f)
        {
            std::vector<Snpt> vNpt = get_vNpt(m_vOrbit[f]);

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

    std::vector<Snpt> OrbitBinReader::getNpt(const char* sFileName_, bool bAllRecord_, bool bIncludeLevels_)
    {
        return std::vector<Snpt>();
    }

    size_t OrbitBinReader::getRecCount(unsigned nIndex_)
    {
        return m_vvNpt[nIndex_].size();
    }

    size_t OrbitBinReader::getFileCount()
    {
        return m_vOrbit.size();
    }

    size_t OrbitBinReader::getCount()
    {
        return m_vvNpt.size();
    }
    std::vector<unsigned> OrbitBinReader::getOrbitListByCoord(float fLatitude_, float fLongitude_)
    {
        std::vector<unsigned> result;

        for (int i = 0; i < m_vOrbit.size(); ++i)
        {
            std::vector<Snpt> vNpt = get_vNpt(m_vOrbit[i]);
            //vThread[i] = std::thread(threadWork, result, m_vOrbit[i], fLatitude_, fLongitude_);

            for (int j = 0; j < vNpt.size(); ++j)
                if (std::abs(vNpt[j].fLatitude - fLatitude_) < 1 && (std::abs(vNpt[j].fLongitude - fLongitude_) < 1))
                {
                    result.push_back(m_vOrbit[i].nOrbit);
                    break;
                }
        }

        //for (unsigned i = 0; i < m_vOrbit.size(); ++i)
        //	vThread[i].join();

        //delete[] vThread;

        return std::move(result);
    }

    unsigned OrbitBinReader::getSpectrumNumb()
    {
        return m_Snpt.nSpectrumNumb;
    }

    float OrbitBinReader::getJulianDate()
    {
        return m_Snpt.fJulianDate;
    }

    float OrbitBinReader::getLocalTime()
    {
        return m_Snpt.fLocalTime;
    }

    float OrbitBinReader::getLS()
    {
        return m_Snpt.fLS;
    }

    std::string OrbitBinReader::getUTC()
    {
        return std::string();
    }

    unsigned OrbitBinReader::getOrbit_by_number(unsigned nNumber_)
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