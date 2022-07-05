#include "pch.h"

#include "Interpolator.h"

#pragma managed(push, off)

namespace orbit
{
	Interpolator::Interpolator(SLevel* pData_, unsigned nCount_)
	{
		m_pData = pData_;
		m_nCount = nCount_;

		if (m_nCount == 0)
			return;

		m_fAltitudeMin = m_pData[0].fAltitude;
		m_fAltitudeMax = m_pData[m_nCount - 1].fAltitude;

		for (unsigned i = 1; i < m_nCount; ++i)
		{
			m_fAltitudeMin = std::min<float>(m_fAltitudeMin, m_pData[i].fAltitude);
			m_fAltitudeMax = std::max<float>(m_fAltitudeMax, m_pData[i].fAltitude);
		}
	}

	void Interpolator::getTemperature(float interpolateMin_, float interpolateMax_, unsigned nInterpolateCount_, std::vector<float>& vTemperature_)
	{
		m_interpolateMin = interpolateMin_;
		m_interpolateMax = std::min<float>(interpolateMax_, m_fAltitudeMax);
		m_nInterpolateCount = nInterpolateCount_;

		vTemperature_.resize(m_nInterpolateCount + 1);

		if (m_nInterpolateCount > 2)
			m_fAltitudeStep = (m_interpolateMax - m_interpolateMin) / (m_nInterpolateCount - 1);
		else
			m_fAltitudeStep = m_interpolateMax - m_interpolateMin;

		vTemperature_[0] = m_pData[0].fTemperature;

		for (unsigned i = 0; i < m_nInterpolateCount; ++i)
			vTemperature_[i + 1] = getTemperature(m_interpolateMin + m_fAltitudeStep * i);
	}

	float Interpolator::getTemperature(float fAltitude_)
	{
		if (fAltitude_ <= m_pData[0].fAltitude)
			return m_pData[0].fTemperature;
		else if (fAltitude_ >= m_pData[m_nCount - 1].fAltitude)
			return m_pData[m_nCount - 1].fTemperature;

		for (unsigned i = 0; i < m_nCount - 1; ++i)
		{
			float t1 = m_pData[i + 0].fTemperature;
			float t2 = m_pData[i + 1].fTemperature;
			float a1 = m_pData[i + 0].fAltitude;
			float a2 = m_pData[i + 1].fAltitude;

			if (fAltitude_ >= a1 && fAltitude_ <= a2)
				return t1 + (t2 - t1) / (a2 - a1) * (fAltitude_ - a1);
		}

		return 0.0f;
	}

	float Interpolator::getAltitudeMin()
	{
		return m_fAltitudeMin;
	}

	float Interpolator::getAltitudeMax()
	{
		return m_fAltitudeMax;
	}

}

#pragma managed(pop)