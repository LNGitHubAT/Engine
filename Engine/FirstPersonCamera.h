#pragma once
#include "Camera.h"
class FirstPersonCamera :
	public Camera
{
public:
	FirstPersonCamera(float FOV, float nearPlane, float farPlane, float screenAspect);
	~FirstPersonCamera();
	void pitch(float angle);
	void yaw(float angle);
	void roll(float angle);
	void direct(float amounth);
	void straft(float amounth);
};

