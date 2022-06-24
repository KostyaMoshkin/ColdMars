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

		const float m_fAngleContinues = 5.0f;    //  ���� � ���������������� ���������� ���� ������ ��������, �� ������ � ���������

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
		virtual size_t getOrbitCount() override;
		virtual std::vector<unsigned> getOrbitListByCoord(float fLatitude_, float fLongitude_) override;
		Snpt getNpt() override;


	public:
		unsigned getOrbitNumber_by_OrbitIndex(unsigned nNumber_) override;
		unsigned getOrbitIndex_by_OrbitNumber(unsigned nNumber_) override;
		unsigned getOrbit_by_LS(unsigned nNumber_) override;
	};
}

