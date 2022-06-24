#pragma once

#include "SLevel.h"
#include "CConfig.h"

namespace orbit
{
	class OrbitBinReader;
	using OrbitBinReaderPtr = std::shared_ptr<OrbitBinReader>;

	class OrbitBinReader : public lib::CConfig
	{
		FILE* m_pLevelFile;

		std::map<int, int> m_mLS;
		std::map<int, int> m_mOrbit;

		std::vector <std::vector<Snpt>> m_vvNpt;
		std::vector<SPairLevel> m_vVertex;

		Snpt m_Snpt;

		unsigned m_nAltitudeMAX = 90;   //  km
		unsigned m_nInterpolateCount = 30;

		const float m_fAngleContinues = 5.0f;    //  Если в последовательных измерениях угол больше значения, то разрыв в рисовании

		std::vector<NptFile> m_vNpt;
		std::vector<OrbitFile> m_vOrbit;

	public:
		OrbitBinReader();
		~OrbitBinReader();

		static OrbitBinReaderPtr Create() { return std::make_shared<OrbitBinReader>(); }

	private:
		std::vector<Snpt> get_vNpt(const OrbitFile& orbit_, bool bAllRecord_);

	public:
		bool init();
		void setFileIndex(unsigned nFirstIndex_, unsigned nLastIndex_, std::vector<SPairLevel>& vLevelData_, bool bClearLevel_ = false);
		size_t getOrbitCount();
		std::vector<unsigned> getOrbitListByCoord(float fLatitude_, float fLongitude_);
		Snpt getNpt();

	public:
		unsigned getOrbitNumber_by_OrbitIndex(unsigned nNumber_);
		unsigned getOrbitIndex_by_OrbitNumber(unsigned nNumber_);
		unsigned getOrbit_by_LS(unsigned nNumber_);
	};
}

