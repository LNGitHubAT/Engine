#pragma once
#include "TankObject.h"
#include "Input.h"
class TankPlayerObject : public TankObject
{
public:
	TankPlayerObject();
	~TankPlayerObject();
	TankPlayerObject(Terrain* terrain, GameObject* AABBdebugObject, AABBCollider m_aabbCollider, Input* input, Mesh* mesh, Shader* shader, Material material, Texture2D * texture,
		ID3D11ShaderResourceView * cubeMapSRV,
		DynamicCubeMap* dynamicCubeMap,
		bool isUseAnphaClipTest = false,
		bool isTransparent = false,
		bool isUseBackCulling = true,
		bool isMirror = false,
		bool isPlane = false);

	virtual void update(float deltaTime) override;
	virtual void handleInput() override;
protected:
	Input* m_input;

};

