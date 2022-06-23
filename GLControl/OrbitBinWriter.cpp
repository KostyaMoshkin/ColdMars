#include "pch.h"
#include "SLevel.h"
#include "OrbitBinWriter.h"

#pragma managed(push, off)

namespace orbit
{
    static std::string file_to_orbit(const std::string& sFile_)
    {
        size_t nPointPos = sFile_.find(".");
        size_t nNamePos = sFile_.find_last_of("\\") + 2;

        return sFile_.substr(nNamePos, nPointPos - nNamePos);
    }


    OrbitBinWriter::OrbitBinWriter()
    {
    }

    OrbitBinWriter::~OrbitBinWriter()
    {
    }

    bool OrbitBinWriter::init()
    {

        //--------------------------------------------------------------------------------------------

        std::string sOrbitFile;
        if (!lib::XMLreader::getSting(lib::XMLreader::getNode(getConfig(), Key::OrbitFileName()), sOrbitFile))
            sOrbitFile = ".\\Orbit.bin";

        {
            FILE* file;
            if (fopen_s(&file, sOrbitFile.c_str(), "r") == 0) {
                fclose(file);
                return true;
            }
        }

        FILE* pOrbitFile;
        if (fopen_s(&pOrbitFile, sOrbitFile.c_str(), "wb") != 0)
            return false;

        //--------------------------------------------------------------------------------------------

        std::string sNptFile;
        if (!lib::XMLreader::getSting(lib::XMLreader::getNode(getConfig(), Key::NptFileName()), sNptFile))
            sNptFile = ".\\Npt.bin";

        FILE* pNptFile;
        if (fopen_s(&pNptFile, sNptFile.c_str(), "wb") != 0)
            return false;

        //--------------------------------------------------------------------------------------------

        std::string sLevelFile;
        if (!lib::XMLreader::getSting(lib::XMLreader::getNode(getConfig(), Key::LevelFileName()), sLevelFile))
            sLevelFile = ".\\Level.bin";

        FILE* pLevelFile;
        if (fopen_s(&pLevelFile, sLevelFile.c_str(), "wb") != 0)
            return false;

        //--------------------------------------------------------------------------------------------

        m_pOrbitTextReader = orbit::OrbitTextReader::Create();
        m_pOrbitTextReader->setConfig(getConfig());

        if (!m_pOrbitTextReader->init())
            return false;

        //--------------------------------------------------------------------------------------------

        std::string sOrbitDir;
        if (!lib::XMLreader::getSting(lib::XMLreader::getNode(getConfig(), Key::OrbitDir()), sOrbitDir))
        {
            toLog("ERROR! Can't find orbit dir tag in config");
            return false;
        }

        std::vector<std::string> vFileList = lib::create_file_list(sOrbitDir.c_str());

        //--------------------------------------------------------------------------------------------

        long nNptPosition = 0;
        long nLevelPosition = 0;

        for (int i = 0; i < vFileList.size(); ++i)
        {
            std::vector<Snpt> vNpt = m_pOrbitTextReader->getNpt(vFileList[i].c_str());

            OrbitFile orbitFile;
            orbitFile.nBegin = nNptPosition;

            std::vector<NptFile> vNptFile;
            std::vector<SLevel> vLevel;

            for (int j = 0; j < vNpt.size(); ++j)
            {
                NptFile nptFile;
                nptFile = vNpt[j];

                nptFile.nBegin = nLevelPosition;
                nLevelPosition += nptFile.nLevelCount * sizeof(SLevel);
                nptFile.nEnd = nLevelPosition;

                vNptFile.push_back(nptFile);

                for(const auto& level : vNpt[j].vLevel)
                    vLevel.push_back(level);

                nNptPosition += sizeof(NptFile);
            }

            orbitFile.nOrbit = vNpt[0].nOrbit;
            orbitFile.nEnd = nNptPosition;

            if (fwrite(&orbitFile, sizeof(OrbitFile), 1, pOrbitFile) != 1)
                return false;

            if (fwrite(vNptFile.data(), vNptFile.size() * sizeof(NptFile), 1, pNptFile) != 1)
                return false;
           
            if (fwrite(vLevel.data(), vLevel.size() * sizeof(SLevel), 1, pLevelFile) != 1)
                return false;

        }

        fclose(pNptFile);
        fclose(pOrbitFile);
        fclose(pLevelFile);

        return true;
    }
}

#pragma managed(pop)