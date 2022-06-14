#version 330 core
layout(location = 0) in float m_fTemperature;

uniform float m_fAltitudeMinMax;

uniform float m_fAltitudeStep;

uniform float m_fDistance_begin;
uniform float m_fDistance_end;

uniform float m_fLatitude_begin;
uniform float m_fLatitude_end;

uniform float m_fLongitude_begin;
uniform float m_fLongitude_end;

uniform float m_fPaletteValueMin;
uniform float m_fPaletteValueMax;

uniform float m_fScale;
uniform int   m_nBaseHeight;

uniform mat4 m_mView;
uniform mat4 m_mRotate;
uniform mat4 m_mTranslate;
uniform mat4 m_mPerspective;

smooth out float fPaletteIndex;

void main()
{
	bool bBeginLevel = gl_VertexID % 2 == 0;

	float fAltitudeMin = bBeginLevel ? m_fDistance_begin : m_fDistance_end;

	int nAltitudeOffetNumber = gl_VertexID / 2;

	float fLatitude		= bBeginLevel ? m_fLatitude_begin : m_fLatitude_end;
	float fLongitude	= bBeginLevel ? m_fLongitude_begin : m_fLongitude_end;
	float fAltitude		= nAltitudeOffetNumber == 0 ? fAltitudeMin : m_fAltitudeMinMax + m_fAltitudeStep * nAltitudeOffetNumber;

	float fDistance = m_fScale * fAltitude / m_nBaseHeight + 1.0;

	vec3 vPosition = fDistance * vec3(
		cos(fLatitude) * sin(fLongitude),
		sin(fLatitude),
		cos(fLatitude) * cos(fLongitude));

	gl_Position = m_mTranslate * m_mPerspective * m_mView * m_mRotate * vec4(vPosition, 1.0);

	fPaletteIndex = (m_fTemperature - m_fPaletteValueMin) / (m_fPaletteValueMax - m_fPaletteValueMin);

	fPaletteIndex = max(min(fPaletteIndex, 0.9999), 0.0001);
}