#pragma once
#include <DirectXMath.h>
using namespace DirectX;
#include "Object.h"
class Light : public Object
{
public:
	Light();
	~Light();
	Light(XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT4 specular);
	XMFLOAT4 m_ambient;
	XMFLOAT4 m_diffuse;
	XMFLOAT4 m_specular;
};

