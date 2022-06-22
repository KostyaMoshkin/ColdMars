#pragma once

#include "CConfig.h"
#include "SLevel.h"

#include <memory>
#include <vector>

namespace orbit
{

	class OrbitReader;
	using OrbitReaderPtr = std::shared_ptr<OrbitReader>;

	class OrbitReader : public lib::CConfig
	{

	public:
		static const char* OrbitFileName() { return "OrbitFile"; }
		static const char* NptFileName() { return "NptFile"; }
		static const char* LevelFileName() { return "LevelFile"; }
		static const char* OrbitDir() { return "OrbitDir"; }
		static const char* TemperartureAltitudeMax() { return "TemperartureAltitudeMax"; }
		static const char* TemperatureInterpolateCount() { return "TemperatureInterpolateCount"; }

	public:
		//static OrbitReaderPtr Create() { return std::make_shared<OrbitReader>(); }



	public:
		virtual bool init() = 0;

		virtual void setFileIndex(unsigned nFirstIndex_, unsigned nLastIndex_, std::vector<SPairLevel>& vLevelData_, bool bClearLevel_ = true) = 0;

		virtual std::vector<Snpt> getNpt(const char* sFileName_, bool bAllRecord_ = true, bool bIncludeLevels_ = true) = 0;

		virtual size_t getRecCount(unsigned nIndex_) = 0;

		virtual size_t getFileCount() = 0;

		virtual size_t getCount() = 0;

		virtual std::vector<unsigned> getOrbitListByCoord(float fLatitude_, float fLongitude_) = 0;

		virtual unsigned getSpectrumNumb() = 0;
		virtual float getJulianDate() = 0;
		virtual float getLocalTime() = 0;
		virtual float getLS() = 0;
		virtual std::string getUTC() = 0;

		virtual unsigned getOrbit_by_number(unsigned nNumber_) = 0;

		virtual unsigned getOrbit_by_LS(unsigned nNumber_) = 0;

	};


}