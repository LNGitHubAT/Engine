#pragma once
#include <DirectXMath.h>
using namespace DirectX;
class Object
{
public:
	Object();
	~Object();
	void setPosition(XMFLOAT3 position);
	virtual void rotate(XMFLOAT3 rotation);
	virtual void rotate(XMFLOAT3 axis, float angle);
	XMMATRIX getWorldMatrix();
	XMMATRIX getWorldMatrixInverseTranspose();
	void updateWorldMatrix();
	XMFLOAT3 getPosition();
	XMFLOAT3 getUp();
	XMFLOAT3 getRight();
	XMFLOAT3 getForward();
	
protected:
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	XMFLOAT3 m_forward;
	XMFLOAT3 m_up;
	XMFLOAT3 m_right;
	XMMATRIX m_worldMatrix;
	
};

