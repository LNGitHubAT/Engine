#pragma once
#include "GameObject.h"
#include "AABBCollider.h"
#include <string>
#include <map>
class PhysicsObject : public GameObject
{
public:

	PhysicsObject(AABBCollider aabbCollider,Mesh* mesh, Shader* shader, Material material, Texture2D * texture = NULL,
		ID3D11ShaderResourceView * cubeMapSRV = NULL,
		DynamicCubeMap* dynamicCubeMap = NULL,
		bool isUseAnphaClipTest = false,
		bool isTransparent = false,
		bool isUseBackCulling = true,
		bool isMirror = false,
		bool isPlane = false);
	PhysicsObject();
	~PhysicsObject();
	void onCollitionEnter(std::string tag, float collideTime);
	AABBCollider getAABBCollider();
protected:
	struct CollideInfo
	{
		std::string tag;
		float collideTime;
		CollideInfo(std::string tag, float collideTime) :
			tag(tag),
			collideTime(collideTime)
		{
		}
	};
	AABBCollider m_aabbCollider;
	float a;
	std::vector<CollideInfo> collideList;
	XMFLOAT3 m_velocity;
};

