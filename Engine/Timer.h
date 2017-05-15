#pragma once
#include <Windows.h>

class Timer
{
public:
	Timer();
	~Timer();
	void start();
	void tick();
	float getDelta();
	float getFPS();
private:
	double m_secondsPerCount;
	double m_deltaTime;
	__int64 m_currTime;
	__int64 m_prevTime;
	float m_FPS;
	float m_FPScounter;
	double m_FPStimer;

};

