#pragma once
#include "GameObject.h"
#include "Light.h"
#include "Define.h"
class PointLightObject : public Light
{
public:
	PointLightObject();
	~PointLightObject();
	PointLightObject(XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT4 specular, float range, XMFLOAT3 m_attenuation);
	float m_range;
	XMFLOAT3 m_attenuation;
	PointLight getData();
};

