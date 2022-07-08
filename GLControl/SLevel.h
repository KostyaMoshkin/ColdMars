#pragma once

namespace orbit
{
	struct SLevel
	{
		float fAltitude;
		float fPresure;
		float fTemperature;
	};

	struct OrbitFile
	{
		long nBegin;
		long nEnd;

		unsigned nOrbit;
	};

	struct Snpt
	{
		unsigned nOrbit;
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

	struct NptFile
	{
		long nBegin;
		long nEnd;

		unsigned nSpectrumNumb;
		unsigned nInterferogramID;
		float fJulianDate;
		char  sUTC[24];
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

		NptFile& operator =(const Snpt& snpt_)
		{
			this->nSpectrumNumb = snpt_.nSpectrumNumb;
			this->nInterferogramID = snpt_.nInterferogramID;
			this->fJulianDate = snpt_.fJulianDate;
			memcpy(&this->sUTC, snpt_.sUTC.data(), 23); this->sUTC[23] = 0;
			this->fDistancToSun = snpt_.fDistancToSun;
			this->fLongitude = snpt_.fLongitude;
			this->fLatitude = snpt_.fLatitude;
			this->fLS = snpt_.fLS;
			this->fLocalTime = snpt_.fLocalTime;
			this->fSunZenit = snpt_.fSunZenit;
			this->fObserverZenit = snpt_.fObserverZenit;
			this->fSurfaceTemp = snpt_.fSurfaceTemp;
			this->fDustOpticalDepth = snpt_.fDustOpticalDepth;
			this->fIceOpticalDepth = snpt_.fIceOpticalDepth;
			this->nLevelCount = snpt_.nLevelCount;

			return *this;
		}
	};

	struct SPairLevel
	{
		unsigned nOrbit;

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

}
