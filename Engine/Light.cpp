#include "Light.h"



Light::Light()
{
}


Light::~Light()
{
}

Light::Light(XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT4 specular)
{
	m_ambient = ambient;
	m_diffuse = diffuse;
	m_specular = specular;
}
