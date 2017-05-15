#include "MathUtility.h"



MathUtility::MathUtility()
{
	
}


MathUtility::~MathUtility()
{
}


const float MathUtility::PI_F = 3.14159265358979f;

float MathUtility::clamp(float value, float min, float max)
{
	return (std::max(min, std::min(value, max)));
	
}

float MathUtility::random(float min, float max)
{
	
	return min + (max - min)*((float)(rand()) / (float)(RAND_MAX));
}

void MathUtility::seed()
{
	srand(time(0));
}

float MathUtility::minf(float a, float b)
{
	return a < b ? a : b;
}

float MathUtility::maxf(float a, float b)
{
	return a > b ? a : b;
}

float MathUtility::maximumFloatNumber()
{
	return FLT_MAX;
}

XMFLOAT4* MathUtility::extractFrustum(XMFLOAT4X4 viewProjectionMatrix)
{
	XMFLOAT4 *planes = new XMFLOAT4[6];
	//
	// Left
	//
	//viewProjectionMatrix.(0, 3);
	planes[0].x = viewProjectionMatrix(0, 3) + viewProjectionMatrix(0, 0);
	planes[0].y = viewProjectionMatrix(1, 3) + viewProjectionMatrix(1, 0);
	planes[0].z = viewProjectionMatrix(2, 3) + viewProjectionMatrix(2, 0);
	planes[0].w = viewProjectionMatrix(3, 3) + viewProjectionMatrix(3, 0);
	//
	// Right
	//
	planes[1].x = viewProjectionMatrix(0, 3) - viewProjectionMatrix(0, 0);
	planes[1].y = viewProjectionMatrix(1, 3) - viewProjectionMatrix(1, 0);
	planes[1].z = viewProjectionMatrix(2, 3) - viewProjectionMatrix(2, 0);
	planes[1].w = viewProjectionMatrix(3, 3) - viewProjectionMatrix(3, 0);
	//
	// Bottom
	//
	planes[2].x = viewProjectionMatrix(0, 3) + viewProjectionMatrix(0, 1);
	planes[2].y = viewProjectionMatrix(1, 3) + viewProjectionMatrix(1, 1);
	planes[2].z = viewProjectionMatrix(2, 3) + viewProjectionMatrix(2, 1);
	planes[2].w = viewProjectionMatrix(3, 3) + viewProjectionMatrix(3, 1);
	//
	// Top
	//
	planes[3].x = viewProjectionMatrix(0, 3) - viewProjectionMatrix(0, 1);
	planes[3].y = viewProjectionMatrix(1, 3) - viewProjectionMatrix(1, 1);
	planes[3].z = viewProjectionMatrix(2, 3) - viewProjectionMatrix(2, 1);
	planes[3].w = viewProjectionMatrix(3, 3) - viewProjectionMatrix(3, 1);
	//
	// Near
	//
	planes[4].x = viewProjectionMatrix(0, 2);
	planes[4].y = viewProjectionMatrix(1, 2);
	planes[4].z = viewProjectionMatrix(2, 2);
	planes[4].w = viewProjectionMatrix(3, 2);
	//
	// Far
	//
	planes[5].x = viewProjectionMatrix(0, 3) - viewProjectionMatrix(0, 2);
	planes[5].y = viewProjectionMatrix(1, 3) - viewProjectionMatrix(1, 2);
	planes[5].z = viewProjectionMatrix(2, 3) - viewProjectionMatrix(2, 2);
	planes[5].w = viewProjectionMatrix(3, 3) - viewProjectionMatrix(3, 2);


	for (int i = 0; i < 6; ++i)
	{
		XMVECTOR v = XMPlaneNormalize(XMLoadFloat4(&planes[i]));
		XMStoreFloat4(&planes[i], v);
	}

	return planes;

}

float MathUtility::infinityF()
{
	return std::numeric_limits<float>::infinity();
}
