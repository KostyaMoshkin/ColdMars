#pragma once
#include "CConfig.h"
#include "SLevel.h"

namespace orbit
{
	struct SPairLevel
	{
		float fLatitude_begin;
		float fLatitude_end;

		float fLongitude_begin;
		float fLongitude_end;

		float fDistane_begin;
		float fDistane_end;

		float fAltitudeMinMax;

		float fAltitudeStep;

		std::vector<float> vTemperature;
	};

	struct Snpt
	{
		unsigned nSpectrumNumb;
		unsigned nInterferogramID;
		float fJulianDate;
		std::string sUTC;
		float fDistancToSun;
		float fLongitude;
		float fLatitude;
		float fLS;
		float fLocalTime;
		float fSunZenit;
		float fObserverZenit;
		float fSurfaceTemp;
		float fDustOpticalDepth;
		float fIceOpticalDepth;
		unsigned short nLevelCount;
		std::vector<SLevel> vLevel;
	};

	class OrbitTextReader;
	using OrbitTextReaderPtr = std::shared_ptr<OrbitTextReader>;

	class OrbitTextReader : public lib::CConfig
	{
		std::vector<std::string> m_vFileList;
		std::map<int, int> m_mLS;
		std::map<int, int> m_mOrbit;

		std::vector <std::vector<Snpt>> m_vvNpt;
		std::vector<SPairLevel> m_vVertex;

		Snpt m_Snpt;

		unsigned m_nAltitudeMAX = 90;   //  km
		unsigned m_nInterpolateCount = 30;

		const float m_fAngleContinues = 5.0f;    //  ���� � ���������������� ���������� ���� ������ ��������, �� ������ � ���������

	public:
		static const char* OrbitDir()						{ return "OrbitDir";					}
		static const char* TemperartureAltitudeMax()		{ return "TemperartureAltitudeMax";		}
		static const char* TemperatureInterpolateCount()	{ return "TemperatureInterpolateCount"; }

	public:
		OrbitTextReader();
		~OrbitTextReader();

		static OrbitTextReaderPtr Create() { return std::make_shared<OrbitTextReader>(); }

	public:
		bool init();

		void setFileIndex(unsigned nFirstIndex_, unsigned nLastIndex_, std::vector<SPairLevel>& vLevelData_, bool bClearLevel_ = true);

		std::vector<Snpt> getNpt(const char* sFileName_, bool bAllRecord_ = true, bool bIncludeLevels_ = true);

		size_t getRecCount(unsigned nIndex_);

		size_t getFileCount();

		size_t getCount();

		std::vector<unsigned> getOrbitListByCoord(float fLatitude_, float fLongitude_);

		unsigned getSpectrumNumb();
		float getJulianDate();
		float getLocalTime();
		float getLS();
		std::string getUTC();

		unsigned getOrbit_by_number(unsigned nNumber_);

		unsigned getOrbit_by_LS(unsigned nNumber_);

	};
}