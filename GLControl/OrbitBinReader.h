#pragma once
#include "OrbitReader.h"

namespace orbit
{
	class OrbitBinReader;
	using OrbitBinReaderPtr = std::shared_ptr<OrbitBinReader>;

	class OrbitBinReader : public OrbitReader
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
		virtual bool init() override;
		virtual void setFileIndex(unsigned nFirstIndex_, unsigned nLastIndex_, std::vector<SPairLevel>& vLevelData_, bool bClearLevel_ = false) override;
		virtual std::vector<Snpt> getNpt(const char* sFileName_, bool bAllRecord_ = true, bool bIncludeLevels_ = true) override;
		virtual size_t getRecCount(unsigned nIndex_) override;
		virtual size_t getFileCount() override;
		virtual size_t getCount() override;
		virtual std::vector<unsigned> getOrbitListByCoord(float fLatitude_, float fLongitude_) override;
		virtual unsigned getSpectrumNumb() override;
		virtual float getJulianDate() override;
		virtual float getLocalTime() override;
		virtual float getLS() override;
		virtual std::string getUTC() override;
		virtual unsigned getOrbit_by_number(unsigned nNumber_) override;
		virtual unsigned getOrbit_by_LS(unsigned nNumber_) override;
	};
}

