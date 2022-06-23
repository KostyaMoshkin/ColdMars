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
		std::map<unsigned, lib::XMLnodePtr> m_vMegdrNode;

		std::map<unsigned, std::vector<MSB_INTEGER>> m_mvRadius;
		std::map<unsigned, std::vector<MSB_INTEGER>> m_mvTopography;
		std::map<unsigned, std::vector<unsigned>> m_mvIndeces;
		std::map<unsigned, std::vector<DrawElementsIndirectCommand>> m_mvIndirect;
		std::map<unsigned, unsigned> m_mnLines;
		std::map<unsigned, unsigned> m_mnLineSamples;
		std::map<unsigned, unsigned> m_mnBaseHeight;

		unsigned m_nActiveID = 0;

	public:
		MegdrReader();
		~MegdrReader();

		static MegdrReaderPtr Create() { return std::make_shared<MegdrReader>(); }

	private:
		bool fillMegdr(unsigned nId_ = 1);

		bool readSingleFileData(unsigned nId_, lib::XMLnodePtr xmlActiveMegdr_);
		bool readMultyFileData(unsigned nId_, lib::XMLnodePtr xmlActiveMegdr_);

	public:
		bool init();

		bool changeMedgr(bool bDirection_ = true);

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
