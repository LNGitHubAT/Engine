#include "BulletObject.h"



BulletObject::BulletObject(Terrain *terrain, AABBCollider aabbCollider, Mesh * mesh, Shader * shader, Material material, Texture2D * texture, ID3D11ShaderResourceView * cubeMapSRV, DynamicCubeMap * dynamicCubeMap, bool isUseAnphaClipTest, bool isTransparent, bool isUseBackCulling, bool isMirror, bool isPlane)
:
	m_terrain(terrain),
	PhysicsObject(aabbCollider,mesh, shader, material, texture, cubeMapSRV, dynamicCubeMap, isUseAnphaClipTest, isTransparent, isUseBackCulling, isMirror, isPlane)
{
}

void BulletObject::update(float deltaTime)
{
	
	for (UINT i = 0; i < collideList.size(); i++)
	{
		destroy();
		return;
	}
}

BulletObject::BulletObject()
{
}


BulletObject::~BulletObject()
{
}
