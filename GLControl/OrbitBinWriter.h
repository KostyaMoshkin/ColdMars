#pragma once
#include "OrbitTextReader.h"

namespace orbit
{
	struct OrbitFile
	{
		long nBegin;
		long nEnd;
	};

	struct NptFile : OrbitFile, Snpt
	{
		unsigned short nLevelSize;

		NptFile& operator =(const Snpt& snpt_)
		{
			this->nSpectrumNumb = snpt_.nSpectrumNumb;
			this->nInterferogramID = snpt_.nInterferogramID;
			this->fJulianDate = snpt_.fJulianDate;
			this->sUTC = snpt_.sUTC;
			this->fDistancToSun = snpt_.fDistancToSun;
			this->fLongitude = snpt_.fLongitude;
			this->fLatitude = snpt_.fLatitude;
			this->fLS = snpt_.fLS;
			this->fLocalTime = snpt_.fLocalTime;
			this->fSunZenit = snpt_.fSunZenit;
			this->fObserverZenit = snpt_.fObserverZenit;
			this->fSurfaceTemp = snpt_.fSurfaceTemp;
			this->fDustOpticalDepth = snpt_.fDustOpticalDepth;
			this->fIceOpticalDepth = snpt_.fIceOpticalDepth;
			this->nLevelCount = snpt_.nLevelCount;
			this->nLevelSize = snpt_.nLevelCount;

			return *this;
		}
	};


	class OrbitBinWriter;
	using OrbitBinWriterPtr = std::shared_ptr<OrbitBinWriter>;

	class OrbitBinWriter : public lib::CConfig
	{
		OrbitTextReaderPtr m_pOrbitTextReader;

		std::vector<orbit::SPairLevel> m_vLevelData;

		std::string m_sOrbitFile;
		std::string m_sNptFile;

	public:
		static const char* OrbitFileName()	{ return "OrbitFile";	}
		static const char* NptFileName()	{ return "NptFile";		}

	public:
		OrbitBinWriter();
		~OrbitBinWriter();

		static OrbitBinWriterPtr Create() { return std::make_shared<OrbitBinWriter>(); }

	public:
		bool init();

		bool compile();
	};
}
