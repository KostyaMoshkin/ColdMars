#pragma once
#include "SLevel.h"
#include "CConfig.h"

namespace orbit
{

	class OrbitBinWriter;
	using OrbitBinWriterPtr = std::shared_ptr<OrbitBinWriter>;

	class OrbitBinWriter : public lib::CConfig
	{
	public:
		OrbitBinWriter();
		~OrbitBinWriter();

		static OrbitBinWriterPtr Create() { return std::make_shared<OrbitBinWriter>(); }

	public:
		bool init();
	};
}
