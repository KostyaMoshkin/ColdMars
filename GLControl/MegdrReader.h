#pragma once
#include "CConfig.h"

namespace megdr
{
	typedef  struct
	{
		GLuint  count;
		GLuint  primCount;
		GLuint  firstIndex;
		GLint   baseVertex;
		GLuint  baseInstance;
	} DrawElementsIndirectCommand;

	using MSB_INTEGER = short;

	class MegdrReader;
	using MegdrReaderPtr = std::shared_ptr<MegdrReader>;

	class MegdrReader : public lib::CConfig
	{
		std::vector<MSB_INTEGER> m_vRadius;
		std::vector<MSB_INTEGER> m_vTopography;
		std::vector<unsigned> m_vIndeces;
		std::vector<DrawElementsIndirectCommand> m_vIndirect;
		unsigned m_nLines;
		unsigned m_nLineSamples;
		unsigned m_nBaseHeight;

	public:
		MegdrReader();
		~MegdrReader();

		static MegdrReaderPtr Create() { return std::make_shared<MegdrReader>(); }

	private:
		bool fillMegdr();

		bool readSingleFileData(lib::XMLnodePtr xmlActiveMegdr_);
		bool readMultyFileData(lib::XMLnodePtr xmlActiveMegdr_);

	public:
		bool init();

		void* getRadius();
		void* getTopography();
		void* getIndeces();
		void* getIndirect();

		unsigned getTopographySize();
		unsigned getRadiusSize();
		unsigned getIndecesSize();
		unsigned getIndirectCommandSize();
		unsigned getIndirectSize();

		unsigned getIndecesCount();
		unsigned getLinesCount();
		unsigned getLineSamplesCount();
		unsigned getBaseHeight();
	};
}
