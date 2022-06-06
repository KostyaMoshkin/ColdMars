#pragma once
#include "CConfig.h"
#include "SLevel.h"

#include <vector>

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

	class OrbitReader;
	using OrbitReaderPtr = std::shared_ptr<OrbitReader>;

	class OrbitReader : public lib::CConfig
	{
		std::vector<std::string> m_vFileList;
		std::vector < std::vector<Snpt>> m_vvNpt;
		std::vector<SPairLevel> m_vVertex;

		unsigned m_nFileId = 0;

		unsigned m_nAltitudeMAX = 90;   //  km
		unsigned m_nInterpolateCount = 30;

		const float m_fAngleContinues = 5.0f;    //  ���� � ���������������� ���������� ���� ������ ��������, �� ������ � ���������

	public:
		static const char* OrbitDir()						{ return "OrbitDir";					}
		static const char* TemperartureAltitudeMax()		{ return "TemperartureAltitudeMax";		}
		static const char* TemperatureInterpolateCount()	{ return "TemperatureInterpolateCount"; }

	public:
		OrbitReader();
		~OrbitReader();

		static OrbitReaderPtr Create() { return std::make_shared<OrbitReader>(); }

	public:
		bool init();

		void setFileIndex(unsigned nIndex_, std::vector<SPairLevel>& vLevelData_);

		size_t getRecCount(unsigned nIndex_);

		size_t getFileCount();

		size_t getCount();

		std::vector<Snpt> operator[](unsigned index_);

	private:

	};
}
