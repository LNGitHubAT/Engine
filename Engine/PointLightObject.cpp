#include "PointLightObject.h"



PointLightObject::PointLightObject()
{

}


PointLightObject::~PointLightObject()
{
}

PointLightObject::PointLightObject(XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT4 specular, float range, XMFLOAT3 attenuation)
	:Light(ambient, diffuse, specular)
{
	
	//Light::Light(ambient, diffuse, specular);
	m_range = range;
	m_attenuation = attenuation;
}

PointLight PointLightObject::getData()
{
	PointLight pointLightStructure;
	pointLightStructure.ambient = m_ambient;
	pointLightStructure.attenuation = m_attenuation;
	pointLightStructure.diffuse = m_diffuse;
	pointLightStructure.position = m_position;
	pointLightStructure.range = m_range;
	pointLightStructure.specular = m_specular;
	pointLightStructure.pad = 0;
	return pointLightStructure;
}
