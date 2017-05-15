#include "Object.h"



Object::Object()
{
	m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_forward = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_worldMatrix = XMMatrixIdentity();
}


Object::~Object()
{
}

void Object::setPosition(XMFLOAT3 position)
{
	m_position = position;
	updateWorldMatrix();
}

void Object::rotate(XMFLOAT3 rotation)
{
	m_rotation.x += rotation.x;
	m_rotation.y += rotation.y;
	m_rotation.z += rotation.z;

	XMVECTOR upVector;
	XMVECTOR rightVector;
	XMVECTOR forwardVector;

	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYawFromVector(XMVECTOR(XMLoadFloat3(&rotation)));
	upVector =  XMVector3Transform(XMLoadFloat3(&m_up), rotationMatrix);
	XMStoreFloat3(&m_up, upVector);

	rightVector = XMVector3Transform(XMLoadFloat3(&m_right), rotationMatrix);
	XMStoreFloat3(&m_right, rightVector);

	forwardVector = XMVector3Transform(XMLoadFloat3(&m_forward), rotationMatrix);
	XMStoreFloat3(&m_forward, forwardVector);

	updateWorldMatrix();
}

void Object::rotate(XMFLOAT3 axis, float angle)
{
	XMVECTOR upVector;
	XMVECTOR rightVector;
	XMVECTOR forwardVector;


	XMMATRIX rotationMatrix = XMMatrixRotationAxis(XMLoadFloat3(&axis), angle);

	upVector = XMVector3Transform(XMLoadFloat3(&m_up), rotationMatrix);
	

	rightVector = XMVector3Transform(XMLoadFloat3(&m_right), rotationMatrix);
	

	forwardVector = XMVector3Transform(XMLoadFloat3(&m_forward), rotationMatrix);
	

	XMStoreFloat3(&m_up, upVector);
	XMStoreFloat3(&m_forward, forwardVector);
	XMStoreFloat3(&m_right, rightVector);

	updateWorldMatrix();
}

XMMATRIX Object::getWorldMatrix()
{
	return m_worldMatrix;
}

XMMATRIX Object::getWorldMatrixInverseTranspose()
{
	XMMATRIX result;
	result = XMMatrixInverse(NULL, m_worldMatrix);
	result = XMMatrixTranspose(result);
	return result;
}

void Object::updateWorldMatrix()
{
	XMMATRIX translationMatrix;
	XMMATRIX rotationMatrix;
	translationMatrix = XMMatrixTranslationFromVector(XMVECTOR(XMLoadFloat3(&m_position)));
	//rotationMatrix = XMMatrixRotationRollPitchYawFromVector(XMVECTOR(XMLoadFloat3(&m_rotation)));

	XMFLOAT4X4 rotation = XMFLOAT4X4(
		m_right.x, m_right.y, m_right.z, 0.0f,
		m_up.x, m_up.y, m_up.z, 0.0f,
		m_forward.x, m_forward.y, m_forward.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	rotationMatrix = XMLoadFloat4x4(&rotation);

	//XMStoreFloat4x4(&rotation, rotationMatrix);

	m_worldMatrix = rotationMatrix * translationMatrix;
}

XMFLOAT3 Object::getPosition()
{
	return m_position;
}

XMFLOAT3 Object::getUp()
{
	
	return m_up;
}

XMFLOAT3 Object::getRight()
{
	return m_right;
}

XMFLOAT3 Object::getForward()
{
	return m_forward;
}


