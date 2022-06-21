#pragma once
#include "CConfig.h"

namespace orbit
{
	class OrbitBinReader;
	using OrbitBinReaderPtr = std::shared_ptr<OrbitBinReader>;

	class OrbitBinReader : public lib::CConfig
	{

	public:
		static const char* OrbitFileName()	{ return "OrbitFile";	}
		static const char* NptFileName()	{ return "NptFile";		}
		static const char* LevelFileName()	{ return "LevelFile";	}

	public:
		OrbitBinReader();
		~OrbitBinReader();

		static OrbitBinReaderPtr Create() { return std::make_shared<OrbitBinReader>(); }

	public:
		bool init();
	};
}

