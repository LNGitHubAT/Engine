#include "TankObject.h"



TankObject::TankObject()
{
}

TankObject::TankObject(Terrain * terrain, GameObject* AABBdebugObject, AABBCollider aabbCollider, Mesh * mesh, Shader * shader, Material material, Texture2D * texture, ID3D11ShaderResourceView * cubeMapSRV, DynamicCubeMap * dynamicCubeMap, bool isUseAnphaClipTest, bool isTransparent, bool isUseBackCulling, bool isMirror, bool isPlane)
	:
	m_terrain(terrain),
	aabbDebug(AABBdebugObject),
	PhysicsObject(aabbCollider, mesh, shader, material, texture, cubeMapSRV , dynamicCubeMap, isUseAnphaClipTest, isTransparent, isUseBackCulling, isMirror, isPlane)
{
	m_velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	collideTimePredicted = 1.0f;
	desireHorizontalOrientation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	forward = XMFLOAT3(0.0f, 0.0f, 1.0f);
	up =  XMFLOAT3(0.0f, 1.0f, 0.0f);
	right = XMFLOAT3(1.0f, 0.0f, 0.0f);

	Mesh * AABBmesh = new Mesh();
	
}

TankObject::~TankObject()
{
}

void TankObject::update(float deltaTime)
{
	//regenerateOrientation();


	float collideTime = 1.0f;
	for (UINT i = 0; i < collideList.size(); i++)
	{

		if (
			(this->tag == "Player" && collideList[i].tag == "Enemy")
			||
			(this->tag == "Enemy" && collideList[i].tag == "Player")
			||
			(this->tag == "Enemy" && collideList[i].tag == "Enemy")
			)
		{
			collideTime = MathUtility::minf(collideTime, collideList[i].collideTime);
		}
		else if
			(
				(this->tag == "Player" && collideList[i].tag == "BulletE")
				||
				(this->tag == "Enemy" && collideList[i].tag == "BulletP")
			)
		{
			beAttacked();
		}
	}

	collideList.clear();

	XMVECTOR veclocityV = XMLoadFloat3(&m_velocity);
	//XMStoreFloat3(&m_velocity, veclocityV);
	XMVECTOR positionV = XMLoadFloat3(&m_position) + veclocityV * MathUtility::minf(collideTime, deltaTime);

	if (isRunning)
	{
		//currentSpeed += accelation * deltaTime;
		//currentSpeed = MathUtility::minf(currentSpeed, maxSpeed);
		currentSpeed = maxSpeed;
		currentRotateSpeed = maxRotateSpeed;
	}
	else
	{
		currentRotateSpeed = 0;
		currentSpeed = 0;
	}
	// move
	veclocityV = XMVector3Normalize(XMLoadFloat3(&desireHorizontalOrientation)) * currentSpeed;
	//XMStoreFloat3(&m_velocity, veclocityV);

	//m_aabbCollider.aabb.center = 


	
	// rotate
	XMVECTOR localForwardV = XMVector3Normalize(XMLoadFloat3(&forward));
	XMVECTOR desireV = XMVector3Normalize(XMLoadFloat3(&desireHorizontalOrientation));
	if (currentRotateSpeed > 0.0f)
	{
		float cosin = XMVectorGetX(XMVector3Dot(desireV, localForwardV));
		cosin = MathUtility::clamp(cosin, -1.0f, 1.0f);
		float angle = acos(cosin);
		
		
		XMVECTOR cross = XMVector3Normalize( XMVector3Cross(localForwardV, desireV));

		
		if ( XMVectorGetX(XMVector3Length(cross)) > 0.0f && XMVectorGetX(XMVector3Dot(XMLoadFloat3(&up), cross)) <= 0.0f)
		{
			angle = -angle;
		}

		if (abs(angle) > currentRotateSpeed * deltaTime)
		{
			if (angle < 0)
			{
				angle = -currentRotateSpeed * deltaTime;
			}
			else
			{
				angle = currentRotateSpeed * deltaTime;
			}
		}
		//angle = MathUtility::minf(angle, currentRotateSpeed * deltaTime);
		
		localRotate(angle);
	}
	

	m_forward = forward;
	m_up = up;
	m_right = right;

	XMStoreFloat3(&m_velocity, veclocityV);
	XMStoreFloat3(&m_position, positionV);

	float height;
	XMFLOAT3 normal;
	m_terrain->getHeightAndNormal(m_position.x, m_position.z, height, normal);
	m_position.y = height;

	XMVECTOR rotationAxis = XMVector3Cross(XMLoadFloat3(&m_up), XMLoadFloat3(&normal));
	XMVector3Normalize(rotationAxis);

	float cosinAngle = XMVectorGetX(XMVector3Dot(XMLoadFloat3(&m_up), XMLoadFloat3(&normal)));

	XMFLOAT3 rotationAxisFloat3;
	XMStoreFloat3(&rotationAxisFloat3, rotationAxis);
	if (XMVectorGetX(XMVector3Length(rotationAxis)) > 0.0f)
	{
		rotate(rotationAxisFloat3, acos(cosinAngle));
	}


	XMFLOAT3 AABBPos;
	XMStoreFloat3(&AABBPos, XMLoadFloat3(&getPosition()) + XMLoadFloat3(&m_aabbCollider.aabb.center));

	aabbDebug->setPosition(AABBPos);

	//return AABBCollider(AABB(AABBPos, m_aabbCollider.aabb.extends), m_velocity);

	updateWorldMatrix();
}

void TankObject::render(ID3D11DeviceContext* deviceContext, Camera* cam, PointLightObject* pointLight, Fog fog, bool isLowestQuality)
{
	GameObject::render(deviceContext, cam, pointLight, fog, isLowestQuality);
	aabbDebug->render(deviceContext, cam, pointLight, fog, isLowestQuality);
}


void TankObject::addDirection(Direction d)
{
	m_direction = (Direction)(m_direction | d);
}

void TankObject::beAttacked()
{

}

void TankObject::localRotate(float angle)
{
	XMVECTOR upVector;
	XMVECTOR rightVector;
	XMVECTOR forwardVector;

	XMMATRIX rotationMatrix = XMMatrixRotationAxis(XMLoadFloat3(&up), angle);

	//upVector = XMVector3Transform(XMLoadFloat3(&up), rotationMatrix);

	rightVector = XMVector3Normalize( XMVector3Transform(XMLoadFloat3(&right), rotationMatrix));

	forwardVector = XMVector3Normalize( XMVector3Transform(XMLoadFloat3(&forward), rotationMatrix));

	//XMStoreFloat3(&up, upVector);

	XMStoreFloat3(&forward, forwardVector);
	XMStoreFloat3(&right, rightVector);
}

void TankObject::regenerateOrientation()
{
	switch (m_direction)
	{
		case Up:
		{
			m_forward = XMFLOAT3(0.0f, 0.0f, 1.0f);
			m_right = XMFLOAT3(1.0f, 0.0f, 0.0f);
			m_up = XMFLOAT3(0.0f, 1.0f, 0.0f);
			break;
		}

		case Down:
		{
			m_forward = XMFLOAT3(0.0f, 0.0f, -1.0f);
			m_right = XMFLOAT3(-1.0f, 0.0f, 0.0f);
			m_up = XMFLOAT3(0.0f, 1.0f, 0.0f);
			break;
		}

		case Left:
		{
			m_forward = XMFLOAT3(-1.0f, 0.0f, 0.0f);
			m_right = XMFLOAT3(0.0f, 0.0f, 1.0f);
			m_up = XMFLOAT3(0.0f, 1.0f, 0.0f);
			break;
		}

		case Right:
		{
			m_forward = XMFLOAT3(1.0f, 0.0f, 0.0f);
			m_right = XMFLOAT3(0.0f, 0.0f, -1.0f);
			m_up = XMFLOAT3(0.0f, 1.0f, 0.0f);
			break;
		}
	};
	//veclocity = 
}
