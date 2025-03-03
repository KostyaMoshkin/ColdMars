#version 330 core

layout(location = 0) in int m_nRadius;
layout(location = 1) in int m_nTopography;

uniform float m_fPaletteValueMin;
uniform float m_fPaletteValueMax;
uniform float m_fScale;
uniform int	m_nLines;
uniform int	m_nLineSamples;
uniform int m_nBaseHeight;

uniform int m_nLongitudeMode;

uniform mat4 m_mView;
uniform mat4 m_mRotate;
uniform mat4 m_mTranslate;
uniform mat4 m_mPerspective;

smooth out vec2 vAlbedoCoords;
smooth out vec3 vSurfaceCoords;

void main()
{
	float fAreoid = m_nRadius - m_nTopography;
	float fDistance = m_nLongitudeMode == 1 ? (m_fScale * m_nTopography + fAreoid) / m_nBaseHeight + 1.0 : 0.95;

	float x = float(gl_VertexID % m_nLineSamples) / float(m_nLineSamples);
	float y = float(gl_VertexID / m_nLineSamples) / float(m_nLines - 1);

	vAlbedoCoords = vec2(x, y);

	float fLatitude = 3.1415926 / 2.0 - y * 3.1415926;
	float fLongitude = x * 3.1415926 * 2.0;

	vec3 vPosition = fDistance * vec3(
		cos(fLatitude) * sin(fLongitude),
		sin(fLatitude),
		cos(fLatitude) * cos(fLongitude));

	gl_Position = m_mTranslate * m_mPerspective * m_mView * m_mRotate * vec4(vPosition, 1.0);

	vSurfaceCoords = vec3(x * 360.0 < 180.0 ? x * 360.0 : x * 360.0 - 360.0, 90.0 - y * 180.0, gl_Position.z);
}