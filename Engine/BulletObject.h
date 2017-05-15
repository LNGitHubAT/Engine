#pragma once
#include "PhysicsObject.h"
#include "Terrain.h"
class BulletObject : PhysicsObject
{
public:
	BulletObject(Terrain* terrain, AABBCollider aabbCollider, Mesh* mesh, Shader* shader, Material material, Texture2D * texture = NULL,
		ID3D11ShaderResourceView * cubeMapSRV = NULL,
		DynamicCubeMap* dynamicCubeMap = NULL,
		bool isUseAnphaClipTest = false,
		bool isTransparent = false,
		bool isUseBackCulling = true,
		bool isMirror = false,
		bool isPlane = false);
	virtual void update(float deltaTime) override;
	BulletObject();
	~BulletObject();
private:
	Terrain* m_terrain;
};

