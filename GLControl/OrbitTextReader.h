#pragma once
#include "OrbitReader.h"

namespace orbit
{
	class OrbitTextReader;
	using OrbitTextReaderPtr = std::shared_ptr<OrbitTextReader>;

	class OrbitTextReader : public OrbitReader
	{
		std::vector<std::string> m_vFileList;
		std::map<int, int> m_mLS;
		std::map<int, int> m_mOrbit;

		std::vector <std::vector<Snpt>> m_vvNpt;
		std::vector<SPairLevel> m_vVertex;

		unsigned m_nAltitudeMAX = 90;   //  km
		unsigned m_nInterpolateCount = 30;

		const float m_fAngleContinues = 5.0f;    //  Если в последовательных измерениях угол больше значения, то разрыв в рисовании

	public:
		OrbitTextReader();
		~OrbitTextReader();

		static OrbitTextReaderPtr Create() { return std::make_shared<OrbitTextReader>(); }

	public:
		virtual bool init() override;
		virtual void setFileIndex(unsigned nFirstIndex_, unsigned nLastIndex_, std::vector<SPairLevel>& vLevelData_, bool bClearLevel_ = false) override;
		virtual std::vector<Snpt> getNpt(const char* sFileName_, bool bAllRecord_ = true, bool bIncludeLevels_ = true) override;
		virtual size_t getOrbitCount() override;
	};
}
