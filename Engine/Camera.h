#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Object.h"
using namespace DirectX;

class Camera 
{
public:
	Camera(float FOV, float nearPlane, float farPlane, float screenAspect);
	~Camera();
	void reset();
	void setPosition(XMFLOAT3 position);
	XMMATRIX getViewMatrix();
	XMMATRIX getProjectionMatrix();
	void updateViewMatrix();
	XMFLOAT3 getPosition();
	virtual void pitch(float angle);
	virtual void yaw(float angle);
	virtual void roll(float angle);
	virtual void direct(float amouth);
	virtual void straft(float amouth);
protected:
	XMFLOAT3 m_position;
	XMFLOAT3 m_forward;
	XMFLOAT3 m_up;
	XMFLOAT3 m_right;
	XMMATRIX m_viewMatrix;
	XMMATRIX projectionMatrix;
	float m_FOV;
	float m_nearPlane;
	float m_farPlane;
	float m_screenAspect;
	
};

