#pragma once
#include "PhysicsObject.h"
#include "Terrain.h"
class TankObject : public PhysicsObject
{
public:
	enum Direction
	{
		Up = 1,
		Down = 2,
		Right = 4,
		Left = 8,
	};
	TankObject();
	TankObject(Terrain* terrain, GameObject* AABBdebugObject, AABBCollider m_aabbCollider, Mesh* mesh, Shader* shader, Material material, Texture2D * texture,
		ID3D11ShaderResourceView * cubeMapSRV,
		DynamicCubeMap* dynamicCubeMap,
		bool isUseAnphaClipTest = false,
		bool isTransparent = false,
		bool isUseBackCulling = true,
		bool isMirror = false,
		bool isPlane = false);
	~TankObject();
	void update(float deltaTime) override;
	void render(ID3D11DeviceContext* deviceContext, Camera* cam, PointLightObject* pointLight, Fog fog, bool isLowestQuality = false) override;
	void setDirection(Direction d)
	{
		m_direction = d;
	}

	void addDirection(Direction d);
	
	//void removeDirection(Direction d);
	void beAttacked();
protected:

	void localRotate(float angle);


	Terrain* m_terrain;
	float collideTimePredicted;
	Direction m_direction;
	void regenerateOrientation();

	float maxSpeed = 20.0f;
	float currentSpeed = 0.0f;
	float accelation = 10.0f;
	bool isRunning = false;
	XMFLOAT3 desireHorizontalOrientation;
	float maxRotateSpeed = 2.0f * MathUtility::PI_F;
	float currentRotateSpeed = 0.0f;

	XMFLOAT3 forward;
	XMFLOAT3 up;
	XMFLOAT3 right;



	// fucking temp
	GameObject* aabbDebug;
};

