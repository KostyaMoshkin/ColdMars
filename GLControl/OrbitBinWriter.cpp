#include "pch.h"
#include "SLevel.h"
#include "OrbitBinWriter.h"

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
        //if (!lib::XMLreader::getSting(lib::XMLreader::getNode(getConfig(), OrbitFileName()), sOrbitFile))
            sOrbitFile = "E:\\Orbit.bin";

        if (FILE* file = fopen(sOrbitFile.c_str(), "r")) {
            fclose(file);
            return true;
        }

        FILE* pOrbitFile;
        if (fopen_s(&pOrbitFile, sOrbitFile.c_str(), "wb") != 0)
            return false;

        //--------------------------------------------------------------------------------------------

        std::string sNptFile;
        //if (!lib::XMLreader::getSting(lib::XMLreader::getNode(getConfig(), NptFileName()), sNptFile))
            sNptFile = "E:\\Npt.bin";

        FILE* pNptFile;
        if (fopen_s(&pNptFile, sNptFile.c_str(), "wb") != 0)
            return false;

        //--------------------------------------------------------------------------------------------

        std::string sLevelFile;
        //if (!lib::XMLreader::getSting(lib::XMLreader::getNode(getConfig(), LevelFileName()), sLevelFile))
            sLevelFile = "E:\\Level.bin";

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
        if (!lib::XMLreader::getSting(lib::XMLreader::getNode(getConfig(), OrbitTextReader::OrbitDir()), sOrbitDir))
        {
            toLog("ERROR! Can't find orbit dir tag in config");
            return false;
        }

        std::vector<std::string> vFileList = lib::create_file_list(sOrbitDir.c_str());

        //--------------------------------------------------------------------------------------------

        long nLevelStartPosition = 0;

        for (int i = 0; i < vFileList.size(); ++i)
        {
            std::vector<Snpt> vNpt = m_pOrbitTextReader->getNpt(vFileList[i].c_str());
            unsigned nOrbit = std::stoi(file_to_orbit(vFileList[i].c_str()));

            long nNptStartPosition = ftell(pNptFile);

            for (int j = 0; j < vNpt.size(); ++j)
            {
                NptFile nptFile;
                nptFile = vNpt[j];
                nptFile.nBegin = nLevelStartPosition;

                nLevelStartPosition += nptFile.nLevelCount * sizeof(SLevel);
                nptFile.nEnd = nLevelStartPosition;

                if (fwrite(&nptFile, sizeof(NptFile), 1, pNptFile) != 1)
                    return false;

                if (fwrite(&vNpt[j].vLevel[0], sizeof(vNpt[j].vLevel) * sizeof(SLevel), 1, pLevelFile) != 1)
                    return false;
            }

            long nNptEndPosition = ftell(pNptFile);

            OrbitFile orbitFile;
            orbitFile.nBegin = nNptStartPosition;
            orbitFile.nEnd = nNptEndPosition;

            if (fwrite(&orbitFile, sizeof(OrbitFile), 1, pOrbitFile) != 1)
                return false;

        }

        fclose(pNptFile);
        fclose(pOrbitFile);
        fclose(pLevelFile);

        return true;
    }

    bool OrbitBinWriter::compile()
    {
        return false;
    }
}