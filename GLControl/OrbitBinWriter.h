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

	};


	class OrbitBinWriter;
	using OrbitBinWriterPtr = std::shared_ptr<OrbitBinWriter>;

	class OrbitBinWriter : public lib::CConfig
	{
		OrbitTextReaderPtr m_pOrbitTextReader;

		std::vector<orbit::SPairLevel> m_vLevelData;

		std::map<int, OrbitFile> m_mOrbitFile;
		std::map<int, NptFile> m_mNptFile;

		std::string m_sOrbitFile;
		std::string m_sNptFile;

	public:
		OrbitBinWriter();
		~OrbitBinWriter();

		static OrbitBinWriterPtr Create() { return std::make_shared<OrbitBinWriter>(); }

	public:
		bool init();

		bool compile();
	};
}
