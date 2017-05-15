#pragma once
#include <limits>
#include "MathUtility.h"


class AABB
{
public:
	XMFLOAT3 center;
	XMFLOAT3 extends;
	AABB()
	{
		center = XMFLOAT3(0.0f, 0.0f, 0.0f);
		extends = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}
	AABB(XMFLOAT3 center, XMFLOAT3 extends)
		:
		center(center),
		extends(extends)
	{
	}

	bool isAABBintersection(AABB another)
	{
		if (
				(center.x + extends.x) < (another.center.x - extends.x) ||
				(center.y + extends.y) < (another.center.y - extends.y) ||
				(center.z + extends.z) < (another.center.z - extends.z) ||

				(center.x - extends.x) > (another.center.x + extends.x) ||
				(center.y - extends.y) > (another.center.y + extends.y) ||
				(center.z - extends.z) > (another.center.z + extends.z)
			)
			{
				return false;
			}
		return true;
	}
};


class AABBCollider
{
public:

	AABBCollider();
	~AABBCollider();
	AABBCollider(AABB aabb, XMFLOAT3 velocity)
		:
		aabb(aabb),
		velocity(velocity)
	{
	}
	AABB aabb;
	XMFLOAT3 velocity;
	// return collide Time
	float sweepAABB(AABBCollider another)
	{

		XMFLOAT3 axisEntryDistance;
		XMFLOAT3 axisExitDistance;

		XMVECTOR velocityV = XMLoadFloat3(&velocity) - XMLoadFloat3(&another.velocity);

		 
		if (XMVectorGetX(XMVector3Length(velocityV)) > 0.0f)
		{
			int x = 10;
		}

		XMFLOAT3 relativeVelocity;
		XMStoreFloat3(&relativeVelocity, velocityV);

		// find the distance between the objects on the near and far sides for both x and y
		if (relativeVelocity.x > 0)
		{
			
			axisEntryDistance.x = (another.aabb.center.x - another.aabb.extends.x) - (aabb.center.x + aabb.extends.x);
			axisExitDistance.x = (another.aabb.center.x + another.aabb.extends.x) - (aabb.center.x - aabb.extends.x);

			if (abs(axisEntryDistance.x) < 0.000002f)
			{
				axisEntryDistance.x = 0.0f;
			}



			
		}

		else
		{
			axisEntryDistance.x = (another.aabb.center.x + another.aabb.extends.x) - (aabb.center.x - aabb.extends.x);
			axisExitDistance.x = (another.aabb.center.x - another.aabb.extends.x) - (aabb.center.x + aabb.extends.x);
		}

		if (relativeVelocity.y > 0)
		{
			axisEntryDistance.y = (another.aabb.center.y - another.aabb.extends.y) - (aabb.center.y + aabb.extends.y);
			axisExitDistance.y = (another.aabb.center.y + another.aabb.extends.y) - (aabb.center.y - aabb.extends.y);
		}
		else
		{
			axisEntryDistance.y = (another.aabb.center.y + another.aabb.extends.y) - (aabb.center.y - aabb.extends.y);
			axisExitDistance.y = (another.aabb.center.y - another.aabb.extends.y) - (aabb.center.y + aabb.extends.y);
		}

		if (relativeVelocity.z > 0)
		{
			axisEntryDistance.z = (another.aabb.center.z - another.aabb.extends.z) - (aabb.center.z + aabb.extends.z);
			axisExitDistance.z = (another.aabb.center.z + another.aabb.extends.z) - (aabb.center.z - aabb.extends.z);
		}
		else
		{
			axisEntryDistance.z = (another.aabb.center.z + another.aabb.extends.z) - (aabb.center.z - aabb.extends.z);
			axisExitDistance.z = (another.aabb.center.z - another.aabb.extends.z) - (aabb.center.z + aabb.extends.z);
		}





		// find time of collision and time of leaving for each axis (if statement is to prevent divide by zero)
		XMFLOAT3 axisEntryTime;
		XMFLOAT3 axisExitTime;

		if (relativeVelocity.x == 0.0f)
		{
			axisEntryTime.x = -std::numeric_limits<float>::infinity();
			axisExitTime.x = std::numeric_limits<float>::infinity();
		}
		else
		{
			axisEntryTime.x = axisEntryDistance.x / relativeVelocity.x;
			axisExitTime.x = axisExitDistance.x / relativeVelocity.x;
		}

		if (relativeVelocity.y == 0.0f)
		{
			axisEntryTime.y = -std::numeric_limits<float>::infinity();
			axisExitTime.y = std::numeric_limits<float>::infinity();
		}
		else
		{
			axisEntryTime.y = axisEntryDistance.y / relativeVelocity.y;
			axisExitTime.y = axisExitDistance.y / relativeVelocity.y;
		}


		if (relativeVelocity.z == 0.0f)
		{
			axisEntryTime.z = -std::numeric_limits<float>::infinity();
			axisExitTime.z = std::numeric_limits<float>::infinity();
		}
		else
		{
			axisEntryTime.z = axisEntryDistance.z / relativeVelocity.z;
			axisExitTime.z = axisExitDistance.z / relativeVelocity.z;
		}

		float entryTime = MathUtility::maxf(MathUtility::maxf(axisEntryTime.x, axisEntryTime.y), axisEntryTime.z);
		float exitTime = MathUtility::minf(MathUtility::minf(axisExitTime.x, axisExitTime.y), axisExitTime.z);

		if (entryTime > exitTime || (axisEntryTime.x < 0.0f && axisEntryTime.x < 0.0f && axisEntryTime.z < 0.0f)
			|| axisEntryTime.x > 1.0f
			|| axisEntryTime.y > 1.0f
			|| axisEntryTime.z > 1.0f
			)
		{
			return 1.0f;
		}
		else
		{
			return entryTime;
		}

	}
	AABB getAABBBoardPhrase()
	{
		XMFLOAT3 center;
		XMFLOAT3 extends;
		XMVECTOR offset = (XMLoadFloat3(&velocity) * 0.5f);


		XMStoreFloat3(&center, XMLoadFloat3(&aabb.center) + offset);
		XMStoreFloat3(&extends, XMLoadFloat3(&aabb.extends) + XMVectorAbs(offset));
		return AABB(center, extends);
	}
};

