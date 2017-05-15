#include "Camera.h"





Camera::Camera(float FOV, float nearPlane, float farPlane, float screenAspect)
{
	this->m_FOV = FOV;
	this->m_nearPlane = nearPlane;
	this->m_farPlane = farPlane;
	this->m_screenAspect = screenAspect;

	m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_forward = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_viewMatrix = XMMatrixIdentity();
	projectionMatrix = XMMatrixPerspectiveFovLH(FOV, screenAspect, nearPlane, farPlane);
	//projectionMatrix = XMMatrixOrthographicLH(800, 600, nearPlane, farPlane);
}

Camera::~Camera()
{
}

void Camera::reset()
{
	m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_forward = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_viewMatrix = XMMatrixIdentity();
}

void Camera::setPosition(XMFLOAT3 position)
{
	//Object::setPosition(position);
	m_position = position;
	updateViewMatrix();
}

XMMATRIX Camera::getViewMatrix()
{
	return m_viewMatrix;
}

XMMATRIX Camera::getProjectionMatrix()
{
	return  projectionMatrix;
}

void Camera::updateViewMatrix()
{
	m_viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&m_position), XMLoadFloat3(&m_forward) + XMLoadFloat3(&m_position), XMLoadFloat3(&m_up));
}

XMFLOAT3 Camera::getPosition()
{
	return m_position;
}

void Camera::pitch(float angle)
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

void Camera::yaw(float angle)
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

void Camera::roll(float angle)
{
	XMVECTOR forwardVector;
	XMVECTOR upVector;
	XMVECTOR rightVector;
	XMFLOAT3 worldYAxis(0.0f, 1.0f, 0.0f);
	//XMVECTOR rotationRightAxis = XMVector3Cross(XMLoadFloat3(&worldYAxis), XMLoadFloat3(&m_forward));
	forwardVector = XMLoadFloat3(&m_forward);


	rightVector = XMVector3Transform(
		XMLoadFloat3(&m_right),
		XMMatrixRotationAxis(forwardVector, angle)
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

void Camera::direct(float amouth)
{
}

void Camera::straft(float amounth)
{
}

