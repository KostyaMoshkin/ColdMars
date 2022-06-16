#pragma once
#include "SLevel.h"

namespace orbit
{
	class Interpolator
	{
		SLevel* m_pData = nullptr;
		unsigned m_nCount = 0;

		float m_interpolateMin = 0.0f;
		float m_interpolateMax = 60;
		unsigned m_nInterpolateCount = 30;

		float m_fAltitudeMin = 0;
		float m_fAltitudeMax = 0;
		float m_fAltitudeStep = 1.0f;

		std::vector<float> m_vTemperature;

	public:
		Interpolator(SLevel* pData_, unsigned nCount_);

		void getTemperature(float interpolateMin_, float interpolateMax_, unsigned nInterpolateCount_, std::vector<float>& vTemperature_);
		float getTemperature(float fAltitude_);

		float getAltitudeMin();
		float getAltitudeMax();
	};
}
