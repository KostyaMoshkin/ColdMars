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

	public:
		//static OrbitReaderPtr Create() { return std::make_shared<OrbitReader>(); }



	public:
		virtual bool init() = 0;

		virtual void setFileIndex(unsigned nFirstIndex_, unsigned nLastIndex_, std::vector<SPairLevel>& vLevelData_, bool bClearLevel_ = true) = 0;

		virtual std::vector<Snpt> getNpt(const char* sFileName_, bool bAllRecord_ = true, bool bIncludeLevels_ = true) { return  std::vector<Snpt>(); }

		virtual Snpt getNpt() { return Snpt(); }

		virtual size_t getOrbitCount() = 0;

		virtual std::vector<unsigned> getOrbitListByCoord(float fLatitude_, float fLongitude_) { return std::vector<unsigned>(); }

		virtual unsigned getOrbit_by_number(unsigned nNumber_) { return 0; }

		virtual unsigned getOrbit_by_LS(unsigned nNumber_) { return 0; }

	};


}