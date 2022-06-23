#pragma once
#include "OrbitTextReader.h"
#include "SLevel.h"

namespace orbit
{

	class OrbitBinWriter;
	using OrbitBinWriterPtr = std::shared_ptr<OrbitBinWriter>;

	class OrbitBinWriter : public lib::CConfig
	{
		OrbitTextReaderPtr m_pOrbitTextReader;

		std::string m_sOrbitFile;
		std::string m_sNptFile;

	public:
		OrbitBinWriter();
		~OrbitBinWriter();

		static OrbitBinWriterPtr Create() { return std::make_shared<OrbitBinWriter>(); }

	public:
		bool init();
	};
}
