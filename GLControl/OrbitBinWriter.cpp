#include "pch.h"
#include "OrbitBinWriter.h"

namespace orbit
{

    OrbitBinWriter::OrbitBinWriter()
    {
    }

    OrbitBinWriter::~OrbitBinWriter()
    {
    }

    bool OrbitBinWriter::init()
    {
        m_pOrbitTextReader = orbit::OrbitTextReader::Create();
        m_pOrbitTextReader->setConfig(getConfig());

        if (!m_pOrbitTextReader->init())
            return false;

        FILE* pOrbitFile;
        if (fopen_s(&pOrbitFile, m_sOrbitFile.c_str(), "rb") != 0)
            return false;

        FILE* pNptFile;
        if (fopen_s(&pNptFile, m_sNptFile.c_str(), "rb") != 0)
            return false;

        for (int i = 0; i < m_pOrbitTextReader->getFileCount(); ++i)
        {
            m_pOrbitTextReader->setFileIndex(i, i + 1, m_vLevelData);
            unsigned nOrbit = m_pOrbitTextReader->getOrbit_by_number(i);

            NptFile nptFile;






            long nNptStartPosition = ftell(pNptFile);



            long nNptEndPosition = ftell(pNptFile);

            OrbitFile orbitFile;
            orbitFile.nBegin = nNptStartPosition;
            orbitFile.nEnd = nNptEndPosition;

        }


        return false;
    }

    bool OrbitBinWriter::compile()
    {
        return false;
    }
}