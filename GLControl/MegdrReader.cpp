#include "pch.h"

#include "MegdrReader.h"

#include <windows.h>

#pragma managed(push, off)

namespace megdr
{
	struct SMegdrFile
	{
		unsigned nSample = -1;
		unsigned nLine = -1;
		std::string sFileName;
	};

	static FILE* openMegdrFile(const char* sFileName, long& nFileSize_)
	{
		FILE* pMegdrFile_ = nullptr;
		if (fopen_s(&pMegdrFile_, sFileName, "rb") != 0)
		{
			toLog(std::string("Radius File not found: ") + std::string(sFileName));
			nFileSize_ = -1;
			return nullptr;
		}

		_fseeki64(pMegdrFile_, 0, SEEK_END);
		nFileSize_ = ftell(pMegdrFile_);
		_fseeki64(pMegdrFile_, 0, SEEK_SET);

		return pMegdrFile_;
	}

	static bool loadArray(const char* pFile_, MSB_INTEGER* vDest_, unsigned nPointsInRaw_, unsigned nArraySegment_, unsigned nLines_, unsigned nLineSamples_)
	{
		long nFileSize = -1;
		FILE* pMegdrFile = openMegdrFile(pFile_, nFileSize);

		if (!pMegdrFile)
			return false;

		auto swapInt = [](MSB_INTEGER* a) {	
			struct int_byte {
				unsigned char a, b;
			}* value = (int_byte * )a;
			std::swap<unsigned char>(value->a, value->b);
		};

		for (unsigned i = 0; i < nLines_; ++i)
		{
			if (fread(vDest_ + nArraySegment_ + i * nPointsInRaw_, nLineSamples_ * sizeof(megdr::MSB_INTEGER), 1, pMegdrFile) != 1)
			{
				toLog("Radius didn't read properly.");
				fclose(pMegdrFile);
				return false;
			}

			for (size_t k = 0; k < nLineSamples_; ++k)
				swapInt(vDest_ + k + nArraySegment_ + i * nPointsInRaw_);
		}

		fclose(pMegdrFile);

		return true;
	}

	static bool readSectorData(MSB_INTEGER* vRadius_, MSB_INTEGER* vTopography_,
		const char *sRadiusPath_, const char* sTopographyPath_,
		lib::XMLnodePtr xmlActiveMegdr_, 
		unsigned nFistLine_ = 0, unsigned nFirstSample_ = 0, unsigned nRowCount_ = 1)
	{
		unsigned nLines = 0;
		lib::XMLreader::getInt(lib::XMLreader::getNode(xmlActiveMegdr_, Key::nLines()), nLines);

		unsigned nLineSamples = 0;
		lib::XMLreader::getInt(lib::XMLreader::getNode(xmlActiveMegdr_, Key::nLineSamples()), nLineSamples);
		
		unsigned nPointsInRaw = nRowCount_ * nLineSamples;
		unsigned nArraySegment = nPointsInRaw * nLines * nFistLine_ * (nRowCount_ - 1) + nFirstSample_ * nLineSamples;

		bool bError = false;

		bError |= loadArray(sRadiusPath_, vRadius_, nPointsInRaw, nArraySegment, nLines, nLineSamples);
		bError |= loadArray(sTopographyPath_, vTopography_, nPointsInRaw, nArraySegment, nLines, nLineSamples);

		return true;
	}

	//---------------------------------------------------------------------------------------------

	MegdrReader::MegdrReader()
	{
	}

	MegdrReader::~MegdrReader()
	{
	}

	bool MegdrReader::init()
	{
		if (!fillMegdr())
			return false;

		return true;
	}

	bool MegdrReader::fillMegdr()
	{
		lib::XMLnodePtr xmlActiveMegdr = lib::XMLreader::getNode(getConfig(), Key::sMegdr());

		//--------------------------------------------------------------------------------------

		bool bXMLmistake = false;

		bXMLmistake |= !lib::XMLreader::getInt(lib::XMLreader::getNode(xmlActiveMegdr, Key::nLines()), m_nLines);
		bXMLmistake |= !lib::XMLreader::getInt(lib::XMLreader::getNode(xmlActiveMegdr, Key::nLineSamples()), m_nLineSamples);

		if (bXMLmistake)
		{
			toLog("Some nodes missed in config file. There should be:  <Lines> <LineSamples>");
			return false;
		}

		//--------------------------------------------------------------------------------------

		unsigned nDataFileCount = 0;
		if (!lib::XMLreader::getInt(lib::XMLreader::getNode(xmlActiveMegdr, Key::sCount()), nDataFileCount))
			nDataFileCount = 0;

		bool bReadFileSuiccess = true;

		if (nDataFileCount > 3)
			bReadFileSuiccess = readMultyFileData(xmlActiveMegdr);
		else
			bReadFileSuiccess = readSingleFileData(xmlActiveMegdr);

		if (!bReadFileSuiccess)
			return false;

		//---------------------------------------------------------------------------------------------

		unsigned nLines = m_nLines;
		unsigned nLineSamples = m_nLineSamples;

		//  Индексы
		m_vIndeces.resize(nLineSamples * 4);

		for (unsigned i = 0; i < nLineSamples; ++i)
		{
			unsigned nTailPoint = i == nLineSamples ? 0 : i + 1;  //  Замкнуть широты
			m_vIndeces[4 * i + 0] = i;
			m_vIndeces[4 * i + 1] = i + nLineSamples;
			m_vIndeces[4 * i + 2] = nTailPoint;
			m_vIndeces[4 * i + 3] = nTailPoint + nLineSamples;
		}

		// Indirect
		m_vIndirect.resize(nLines - 1);
		for (unsigned i = 0; i < nLines - 1; ++i)
		{
			m_vIndirect[i].count = getIndecesCount();
			m_vIndirect[i].primCount = 1;
			m_vIndirect[i].firstIndex = 0;
			m_vIndirect[i].baseVertex = i * nLineSamples;
			m_vIndirect[i].baseInstance = 0;
		}

		//---------------------------------------------------------------------------------------------

		return true;
	}

	bool MegdrReader::readSingleFileData(lib::XMLnodePtr xmlActiveMegdr_)
	{
		try
		{
			m_vRadius.resize(m_nLines * m_nLineSamples);
			m_vTopography.resize(m_nLines * m_nLineSamples);
		}
		catch (...)
		{
			toLog("No enough memory. Ask for: " + std::to_string(m_nLines * m_nLineSamples * 2 / 1024 / 1024) + " x 2 Mb");
			return false;
		}

		std::string sRadiusPath;

		bool bXMLmistake = false;

		bXMLmistake |= !lib::XMLreader::getSting(lib::XMLreader::getNode(xmlActiveMegdr_, Key::sRadiusFile()), sRadiusPath);

		std::string sTopographyPath;
		bXMLmistake |= !lib::XMLreader::getSting(lib::XMLreader::getNode(xmlActiveMegdr_, Key::sTopographyFile()), sTopographyPath);

		if (bXMLmistake)
		{
			toLog("Some nodes missed in config file. There should be: <RadiusFile> <TopographyFile>");
			return false;
		}

		//--------------------------------------------------------------------------------------------

		bool bResult = readSectorData(m_vRadius.data(), m_vTopography.data(), sRadiusPath.c_str(), sTopographyPath.c_str(), xmlActiveMegdr_);

		return bResult;
	}

	bool MegdrReader::readMultyFileData(lib::XMLnodePtr xmlActiveMegdr_)
	{
		unsigned nDataFileCount = 1;
		bool bMultyData = lib::XMLreader::getInt(lib::XMLreader::getNode(xmlActiveMegdr_, Key::sCount()), nDataFileCount);

		unsigned nDataFileCountRaw = 1;

		nDataFileCountRaw = (unsigned)sqrt(nDataFileCount);
		if (nDataFileCountRaw * nDataFileCountRaw != nDataFileCount)
		{
			toLog("Node <Count> should be i^2^ 4, 9, 16, 25 ...");
			return false;
		}
		
		m_nLines *= nDataFileCountRaw;
		m_nLineSamples *= nDataFileCountRaw;

		try
		{
			m_vRadius.resize(m_nLines * m_nLineSamples);
			m_vTopography.resize(m_nLines * m_nLineSamples);
		}
		catch ( ... )
		{
			toLog("No enough memory. Ask for: " + std::to_string(m_nLines * m_nLineSamples * 2 / 1024 / 1024) + " x 2 Mb");
			return false;
		}

		//--------------------------------------------------------------------------------------------

		std::vector<std::pair<SMegdrFile, SMegdrFile>> vMegdrSrs(nDataFileCount);    // first - radius, second - topography

		lib::XMLnodePtr xmlMegdrFile = xmlActiveMegdr_->FirstChild();

		while (!!xmlMegdrFile)
		{
			std::string sNodeName = xmlMegdrFile->Value();

			bool bFileRadius = sNodeName.compare(Key::sRadiusFile()) == 0;
			bool bFileTopography = sNodeName.compare(Key::sTopographyFile()) == 0;

			if (!bFileRadius && !bFileTopography)
			{
				xmlMegdrFile = xmlMegdrFile->NextSibling();
				continue;
			}

			unsigned nLine = -1;
			if (!lib::XMLreader::getInt(xmlMegdrFile, Key::line(), nLine))
				return false;

			unsigned nSample = -1;
			if (!lib::XMLreader::getInt(xmlMegdrFile, Key::sample(), nSample))
				return false;

			std::string sPath;
			lib::XMLreader::getSting(xmlMegdrFile, sPath);

			unsigned nFileIndex = (nLine - 1) * nDataFileCountRaw + (nSample - 1);

			if (bFileRadius)
			{
				vMegdrSrs[nFileIndex].first.sFileName = sPath;
				vMegdrSrs[nFileIndex].first.nLine = nLine - 1;
				vMegdrSrs[nFileIndex].first.nSample = nSample - 1;
			}
			else
			{
				vMegdrSrs[nFileIndex].second.sFileName = sPath;
				vMegdrSrs[nFileIndex].second.nLine = nLine - 1;
				vMegdrSrs[nFileIndex].second.nSample = nSample - 1;
			}

			xmlMegdrFile = xmlMegdrFile->NextSibling();
		}

		//--------------------------------------------------------------------------------------------

		bool bReadMistake = false;

		for (auto& megdrFile : vMegdrSrs)
			if (!megdrFile.first.sFileName.empty())
				bReadMistake |= readSectorData(m_vRadius.data(), m_vTopography.data(),
					megdrFile.first.sFileName.c_str(), megdrFile.second.sFileName.c_str(),
					xmlActiveMegdr_,
					megdrFile.first.nLine, megdrFile.first.nSample, nDataFileCountRaw);

		//--------------------------------------------------------------------------------------------

		return !bReadMistake;
	}

	void* MegdrReader::getRadius()
	{
		return m_vRadius.data();
	}

	unsigned MegdrReader::getRadiusSize()
	{
		return unsigned(sizeof(megdr::MSB_INTEGER) * m_vRadius.size());
	}

	void* MegdrReader::getTopography()
	{
		return m_vTopography.data();
	}

	unsigned MegdrReader::getTopographySize()
	{
		return unsigned(sizeof(megdr::MSB_INTEGER) * m_vTopography.size());
	}

	void* MegdrReader::getIndeces()
	{
		return m_vIndeces.data();
	}

	void* MegdrReader::getIndirect()
	{
		return m_vIndirect.data();
	}

	unsigned MegdrReader::getIndecesSize()
	{
		return unsigned(sizeof(unsigned) * getIndecesCount());
	}

	unsigned MegdrReader::getIndecesCount()
	{
		return unsigned(m_vIndeces.size());
	}

	unsigned MegdrReader::getIndirectCommandSize()
	{
		return unsigned(sizeof(megdr::DrawElementsIndirectCommand));
	}

	unsigned MegdrReader::getIndirectSize()
	{
		return unsigned(getIndirectCommandSize()  * m_vIndirect.size());
	}

	unsigned MegdrReader::getLinesCount()
	{
		return m_nLines;
	}

	unsigned MegdrReader::getLineSamplesCount()
	{
		return m_nLineSamples;
	}

	unsigned MegdrReader::getBaseHeight()
	{
		return m_nBaseHeight;
	}

}

#pragma managed(pop)