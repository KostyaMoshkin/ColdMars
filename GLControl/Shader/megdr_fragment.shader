#version 330 core

uniform sampler2D m_tAlbedo;
uniform sampler2D m_tDigit;

uniform vec3 m_vMarsTone;
uniform vec3 m_vDigitColor;
uniform vec3 m_vNetColor;

smooth in vec2 vAlbedoCoords;

ivec2 getNetCoord()
{
	int nLongitude = (int(vAlbedoCoords.x * 360.0) / 30) * 30;
	int nLatitude = (int(vAlbedoCoords.y * 180.0) / 30) * 30;

	return ivec2(nLongitude < 180 ? nLongitude : nLongitude - 360, 90 - nLatitude);
}

vec2 getDigitCoord()
{
	int nAcuracy = 30;
	int nAngleStep = 30;

	float offsetLong = 1.0 * nAcuracy;
	float offsetLat  = 1.0 * nAcuracy;

	float sizeLong = 6.0 * nAcuracy;
	float sizeLat  = 1.5 * nAcuracy;

	int nLongitude = int(vAlbedoCoords.x * 360.0 * nAcuracy);
	int nLatitude = int(vAlbedoCoords.y * 180.0 * nAcuracy);

	float fLongitude = float(nLongitude % (nAngleStep * nAcuracy)) - offsetLong;
	float fLatitude  = float(nLatitude  % (nAngleStep * nAcuracy)) - offsetLat;

	if (
		fLongitude > 0
		&& fLatitude > 0
		&& fLongitude < sizeLong
		&& fLatitude < sizeLat
		) 
		return vec2( fLongitude / sizeLong, fLatitude / sizeLat );

	return vec2(-1.0, -1.0);
}

bool isNet()
{
	int nLongitude = int(vAlbedoCoords.x * 360.0 * 10.0);
	int nLatitude = int(vAlbedoCoords.y * 180.0 * 10.0);

	if (nLongitude < 6)
		return true;

	if ((nLatitude < 903) && (nLatitude > 897))
		return true;

	if ((nLongitude % 300) < 1)
		return true;

	if ((nLatitude % 300) < 1)
		return true;

	return false;
}

float getDigitBrightness(vec2 textDigit_)
{
	ivec2 netCoord = getNetCoord();

	int nDigitPosition = int(textDigit_.x * 8);

	int nDigitToWrite = 0;

	if (nDigitPosition == 0)
	{
		if (netCoord.x < 0)
			nDigitToWrite = 11; //  -
		else
			return 0;
	}
	else if (nDigitPosition == 1)
	{
		nDigitToWrite = abs(netCoord.x) / 100;
		if (nDigitToWrite == 0)
			return 0;
	}
	else if (nDigitPosition == 2)
	{
		nDigitToWrite = (abs(netCoord.x) / 10) % 10;
		if (nDigitToWrite == 0 && netCoord.x / 100 == 0)
			return 0;
	}
	else if (nDigitPosition == 3)
		nDigitToWrite = abs(netCoord.x) % 10;
	else if (nDigitPosition == 4)
		nDigitToWrite = 10; //  :
	else if (nDigitPosition == 5)
	{
		if (netCoord.y < 0)
			nDigitToWrite = 11; //  -
		else
			return 0;
	}
	else if (nDigitPosition == 6)
	{
		nDigitToWrite = abs(netCoord.y / 10);
		if (nDigitToWrite == 0)
			return 0;
	}
	else if (nDigitPosition == 7)
		nDigitToWrite = abs(netCoord.y) % 10;

	float fLetterX = ((textDigit_.x - 1.0 * nDigitPosition / 8) * 8 + nDigitToWrite) / 12;

	return texture(m_tDigit, vec2(fLetterX, textDigit_.y)).r;
}

void main()
{
	if (isNet())
	{
		gl_FragColor = vec4(m_vNetColor, 1.0);
		return;
	}

	vec2 vAlbedo = vec2(vAlbedoCoords.x + 0.5 > 1.0 ? vAlbedoCoords.x - 0.5 : vAlbedoCoords.x + 0.5, vAlbedoCoords.y);

	float fAlbedo = texture(m_tAlbedo, vAlbedo).r;

	if (vAlbedoCoords.y < 0.05 || vAlbedoCoords.y > 0.97)
	{
		if (fAlbedo > 0.8)
		{
			gl_FragColor = vec4(fAlbedo, fAlbedo, fAlbedo, 1.0);
			return;
		}
	}

	vec2 vDigitCoord = getDigitCoord();

	if (vDigitCoord.x >= 0)
	{
		float fDigitBrightness = getDigitBrightness(vDigitCoord);

		vec3 color = (m_vMarsTone * fAlbedo) * (1 - fDigitBrightness) + m_vDigitColor * fDigitBrightness;
		gl_FragColor = vec4(color, 1.0);
	}
	else
		gl_FragColor = vec4(m_vMarsTone * fAlbedo, 1.0);
}