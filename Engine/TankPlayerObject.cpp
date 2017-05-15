#include "TankPlayerObject.h"



TankPlayerObject::TankPlayerObject()
{
}


TankPlayerObject::~TankPlayerObject()
{
}

TankPlayerObject::TankPlayerObject(Terrain * terrain, GameObject* AABBdebugObject, AABBCollider m_aabbCollider,  Input * input, Mesh * mesh, Shader * shader, Material material, Texture2D * texture, ID3D11ShaderResourceView * cubeMapSRV, DynamicCubeMap * dynamicCubeMap, bool isUseAnphaClipTest, bool isTransparent, bool isUseBackCulling, bool isMirror, bool isPlane)
	:
	m_input(input),
	TankObject(terrain, AABBdebugObject, m_aabbCollider, mesh, shader, material, texture, cubeMapSRV, dynamicCubeMap, isUseAnphaClipTest, isTransparent, isUseBackCulling, isMirror, isPlane)
{

}

void TankPlayerObject::update(float deltaTime)
{
	
	TankObject::update(deltaTime);
}

void TankPlayerObject::handleInput()
{
	std::vector<KeyEvent> keyEventQueue = m_input->getKeyBuffer();
	XMVECTOR desireV = XMLoadFloat3(&desireHorizontalOrientation);
	for (UINT i = 0; i < keyEventQueue.size(); i++)
	{
		KeyEvent e = keyEventQueue[i];

		if (e.getCode() == VK_UP && e.isPress())
		{
			desireV += XMLoadFloat3(&(XMFLOAT3(0.0f, 0.0f, 1.0f)));
		}
		else if (e.getCode() == VK_UP && e.isRelease())
		{
			desireV -= XMLoadFloat3(&(XMFLOAT3(0.0f, 0.0f, 1.0f)));
		}
		else if (e.getCode() == VK_DOWN && e.isPress())
		{
			desireV += XMLoadFloat3(&(XMFLOAT3(0.0f, 0.0f, -1.0f)));
		}
		else if (e.getCode() == VK_DOWN && e.isRelease())
		{
			desireV -= XMLoadFloat3(&(XMFLOAT3(0.0f, 0.0f, -1.0f)));
		}
		else if (e.getCode() == VK_LEFT && e.isPress())
		{
			desireV += XMLoadFloat3(&(XMFLOAT3(-1.0f, 0.0f, 0.0f)));
		}
		else if (e.getCode() == VK_LEFT && e.isRelease())
		{
			desireV -= XMLoadFloat3(&(XMFLOAT3(-1.0f, 0.0f, 0.0f)));
		}
		else if (e.getCode() == VK_RIGHT && e.isPress())
		{
			desireV += XMLoadFloat3(&(XMFLOAT3(1.0f, 0.0f, 0.0f)));
		}
		else if (e.getCode() == VK_RIGHT && e.isRelease())
		{
			desireV -= XMLoadFloat3(&(XMFLOAT3(1.0f, 0.0f, 0.0f)));
		}

	}

	if (XMVectorGetX(XMVector3Length(desireV)) > 0.0f)
	{
		isRunning = true;
	}
	else
	{
		isRunning = false;
	}

	XMStoreFloat3(&desireHorizontalOrientation, desireV);


}
