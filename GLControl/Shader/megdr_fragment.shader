#version 330 core

uniform sampler2D m_tAlbedo;
//uniform sampler2D m_tDigit;

uniform vec3 m_vMarsTone;
uniform vec3 m_vDigitColor;

smooth in vec2 vAlbedoCoords;

bool isNet()
{
	int nLongitude = int(vAlbedoCoords.x * 360.0 * 10.0);
	int nLatitude = int(vAlbedoCoords.y * 180.0 * 10.0);

	if (nLongitude < 6)
		return true;

	if ((nLatitude < 903) && (nLatitude > 897))
		return true;

	if ((nLongitude % 300) == 0)
		return true;

	if ((nLatitude % 300) == 0)
		return true;

	return false;
}

void main()
{
	if (isNet())
	{
		gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
		return;
	}

	vec2 vAlbedo = vec2(vAlbedoCoords.x + 0.5 > 1.0 ? vAlbedoCoords.x - 0.5 : vAlbedoCoords.x + 0.5, vAlbedoCoords.y);

	float fAlbedo = texture(m_tAlbedo, vAlbedo).r;

	if (vAlbedoCoords.y < 0.05 || vAlbedoCoords.y > 0.97)
	{
		//  Polar ice
		if (fAlbedo > 0.8)
		{
			gl_FragColor = vec4(fAlbedo, fAlbedo, fAlbedo, 1.0);
			return;
		}
	}

	gl_FragColor = vec4(m_vMarsTone * fAlbedo, 1.0);
}