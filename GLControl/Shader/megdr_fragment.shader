#version 330 core

uniform sampler2D m_tAlbedo;

uniform vec3 m_vMarsTone;

smooth in vec2 vAlbedoCoords;

void main()
{
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