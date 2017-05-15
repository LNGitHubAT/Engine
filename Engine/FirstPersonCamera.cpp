#include "FirstPersonCamera.h"



FirstPersonCamera::FirstPersonCamera(float FOV, float nearPlane, float farPlane, float screenAspect)
	:
	Camera(FOV,  nearPlane, farPlane, screenAspect)
{
}


FirstPersonCamera::~FirstPersonCamera()
{
}

void FirstPersonCamera::pitch(float angle)
{
	XMVECTOR forwardVector;
	XMVECTOR upVector;
	XMVECTOR rightVector;
	XMFLOAT3 worldYAxis(0.0f, 1.0f, 0.0f);
	//rightVector = XMVector3Cross(XMLoadFloat3(&worldYAxis), XMLoadFloat3(&m_forward));
	rightVector = XMLoadFloat3(&m_right);
	forwardVector = XMVector3Transform(
									XMLoadFloat3(&m_forward),
									XMMatrixRotationAxis(rightVector, angle)
									);


	upVector = XMVector3Cross(forwardVector, rightVector);


	XMStoreFloat3(&m_forward, forwardVector);
	XMStoreFloat3(&m_up, upVector);
	XMStoreFloat3(&m_right, rightVector);

	updateViewMatrix();
}

void FirstPersonCamera::yaw(float angle)
{
	XMVECTOR forwardVector;
	XMVECTOR upVector;
	XMVECTOR rightVector;
	XMFLOAT3 worldYAxis(0.0f, 1.0f, 0.0f);
	//XMVECTOR rotationRightAxis = XMVector3Cross(XMLoadFloat3(&worldYAxis), XMLoadFloat3(&m_forward));
	forwardVector = XMVector3Transform(
		XMLoadFloat3(&m_forward),
		XMMatrixRotationAxis(XMLoadFloat3(&worldYAxis), angle)
	);
	

	rightVector = XMVector3Transform(
		XMLoadFloat3(&m_right),
		XMMatrixRotationAxis(XMLoadFloat3(&worldYAxis), angle)
	);
	//rightVector = XMVector3Cross(XMLoadFloat3(&worldYAxis), forwardVector);

	/*float rightLength = XMVectorGetX(XMVector3Length(rightVector));
	if (rightLength < 1.0f)
		return;*/

	upVector = XMVector3Cross(forwardVector, rightVector);

	XMStoreFloat3(&m_forward, forwardVector);
	XMStoreFloat3(&m_up, upVector);
	XMStoreFloat3(&m_right, rightVector);
	updateViewMatrix();

}

void FirstPersonCamera::roll(float angle)
{

}

void FirstPersonCamera::direct(float amouth)
{
	XMVECTOR positionVector = XMLoadFloat3(&m_position) + XMLoadFloat3(&m_forward) *amouth;
	XMStoreFloat3(&m_position, positionVector);
}

void FirstPersonCamera::straft(float amouth)
{
	XMVECTOR positionVector = XMLoadFloat3(&m_position) + XMLoadFloat3(&m_right) *amouth;
	XMStoreFloat3(&m_position, positionVector);
}
