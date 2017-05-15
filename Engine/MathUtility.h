#pragma once
#include <algorithm>
#include <stdio.h>
#include <ctime>
#include <math.h>
#include <DirectXMath.h>

using namespace DirectX;

class MathUtility
{
public:
	MathUtility();
	~MathUtility();
	
	static float clamp(float value, float min, float max);
	static float random(float min, float max);
	static void seed();
	static float minf(float a, float b);
	static float maxf(float a, float b);
	static float maximumFloatNumber();
	static XMFLOAT4* extractFrustum(XMFLOAT4X4 viewProjectionMatrix);
	static const float PI_F;
	static float infinityF();
};

