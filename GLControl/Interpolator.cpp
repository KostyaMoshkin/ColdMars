#include "pch.h"

#include "Interpolator.h"

#include "lib.h"

#include <algorithm>

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
		m_fAltitudeStep = (m_interpolateMax - m_interpolateMin) / m_nInterpolateCount;

		vTemperature_[0] = m_pData[0].fTempereture;

		for (unsigned i = 0; i < m_nInterpolateCount; ++i)
			vTemperature_[i + 1] = getTemperature(m_interpolateMin + m_fAltitudeStep * i);
	}

	float Interpolator::getTemperature(float fAltitude_)
	{
		if (fAltitude_ <= m_pData[0].fAltitude)
			return m_pData[0].fTempereture;
		else if (fAltitude_ >= m_pData[m_nCount - 1].fAltitude)
			return m_pData[m_nCount - 1].fTempereture;

		for (unsigned i = 0; i < m_nCount - 1; ++i)
		{
			if (fAltitude_ >= m_pData[i].fAltitude && fAltitude_ <= m_pData[i + 1].fAltitude)
				return m_pData[i].fTempereture + (m_pData[i + 1].fTempereture - m_pData[i].fTempereture) / (m_pData[i + 1].fAltitude - m_pData[i].fAltitude) * (m_pData[i].fAltitude - fAltitude_);
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