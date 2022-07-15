#version 430 core
layout(location = 0) in float m_fValue;

struct SLevelCoord
{
	float fLatitude_begin;
	float fLatitude_end;

	float fLongitude_begin;
	float fLongitude_end;

	float fDistance_begin;
	float fDistance_end;

	float fAltitudeMinMax;

	float fAltitudeStep;
};

layout(std430, binding = 0) buffer LevelCoord 
{ 
	SLevelCoord vLevelCoord[];
};

uniform float m_fPaletteValueMin;
uniform float m_fPaletteValueMax;

uniform float m_fScale;
uniform int   m_nBaseHeight;

uniform int   m_nLevelIndex;

uniform mat4 m_mView;
uniform mat4 m_mRotate;
uniform mat4 m_mTranslate;
uniform mat4 m_mPerspective;

smooth out float fPaletteIndex;

void main()
{
	bool bBeginLevel = gl_VertexID % 2 == 0;

	float fAltitudeMin = bBeginLevel ? vLevelCoord[m_nLevelIndex].fDistance_begin : vLevelCoord[m_nLevelIndex].fDistance_end;

	int nAltitudeOffetNumber = gl_VertexID / 2;

	float fLatitude		= bBeginLevel ? vLevelCoord[m_nLevelIndex].fLatitude_begin : vLevelCoord[m_nLevelIndex].fLatitude_end;
	float fLongitude	= bBeginLevel ? vLevelCoord[m_nLevelIndex].fLongitude_begin : vLevelCoord[m_nLevelIndex].fLongitude_end;
	float fAltitude		= nAltitudeOffetNumber == 0 ? fAltitudeMin : vLevelCoord[m_nLevelIndex].fAltitudeMinMax + vLevelCoord[m_nLevelIndex].fAltitudeStep * nAltitudeOffetNumber;

	float fDistance = m_fScale * fAltitude / m_nBaseHeight + 1.0;

	vec3 vPosition = fDistance * vec3(
		cos(fLatitude) * sin(fLongitude),
		sin(fLatitude),
		cos(fLatitude) * cos(fLongitude));

	gl_Position = m_mTranslate * m_mPerspective * m_mView * m_mRotate * vec4(vPosition, 1.0);

	fPaletteIndex = (m_fValue - m_fPaletteValueMin) / (m_fPaletteValueMax - m_fPaletteValueMin);

	fPaletteIndex = max(min(fPaletteIndex, 0.9999), 0.0001);
}