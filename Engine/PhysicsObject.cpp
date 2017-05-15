#include "PhysicsObject.h"



PhysicsObject::PhysicsObject(AABBCollider aabbCollider, Mesh * mesh, Shader * shader, Material material, Texture2D * texture, ID3D11ShaderResourceView * cubeMapSRV, DynamicCubeMap * dynamicCubeMap, bool isUseAnphaClipTest, bool isTransparent, bool isUseBackCulling, bool isMirror, bool isPlane)
	:
	m_aabbCollider(aabbCollider),
	GameObject(mesh, shader, material, texture, cubeMapSRV, dynamicCubeMap, isUseAnphaClipTest, isTransparent, isUseBackCulling, isMirror, isPlane)
{
}

PhysicsObject::PhysicsObject()
{
}


PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::onCollitionEnter(std::string tag, float collideTime)
{
	collideList.push_back(CollideInfo(tag, collideTime));
}

AABBCollider PhysicsObject::getAABBCollider()
{
	XMFLOAT3 AABBPos;
	XMStoreFloat3(&AABBPos, XMLoadFloat3(&getPosition()) + XMLoadFloat3(&m_aabbCollider.aabb.center));
	
	return AABBCollider(AABB(AABBPos, m_aabbCollider.aabb.extends), m_velocity);
}
