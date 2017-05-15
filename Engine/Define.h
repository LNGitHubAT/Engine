#pragma once
#include <DirectXMath.h>
using namespace DirectX;
const float  PI_F = 3.14159265358979f;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const float FOV = XMConvertToRadians(45.0f);

struct Material
{
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	XMFLOAT4 reflect;
	XMFLOAT4 refract;
	float fresnelPower;
	XMFLOAT3 pad;
};

struct Fog
{
	float range;
	float start;
	XMFLOAT4 color;
};

struct PointLight
{
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	XMFLOAT3 position;
	float range;
	XMFLOAT3 attenuation;
	float pad;
};

struct BasicVertex
{
	BasicVertex()
	{}
	BasicVertex(float  x, float y, float z, float xn, float yn, float zn, float u, float v)
		:
		position(x, y ,z),
		normal(xn, yn, zn),
		textureCoord(u, v)
	{

	}
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 textureCoord;
};

class Vertex
{
public:
	struct BasicVertex
	{
		BasicVertex()
		{}
		BasicVertex(float  x, float y, float z, float xn, float yn, float zn, float u, float v)
			:
			position(x, y, z),
			normal(xn, yn, zn),
			textureCoord(u, v)
		{

		}
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT2 textureCoord;
	};
	struct TerrainVertex
	{
		XMFLOAT3 position;
		XMFLOAT2 textureCoord;
		XMFLOAT2 boundaryY;
	};

	struct SkyBox
	{
		SkyBox() {}
		SkyBox(float x, float y, float z)
			:
			position(x, y , z)
		{

		}
		XMFLOAT3 position;
	};

};
