#include "Timer.h"



Timer::Timer()
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&	countsPerSec);
	m_secondsPerCount = 1 / double(countsPerSec);
	m_prevTime = 0;
	m_currTime = 0;
	m_deltaTime = 0;
	m_FPS = 0;
	m_FPStimer = 0;
	m_FPScounter = 0;
	start();
}


Timer::~Timer()
{
}

void Timer::start()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_prevTime);

}

void Timer::tick()
{

	QueryPerformanceCounter((LARGE_INTEGER*)&m_currTime);
	m_deltaTime = (m_currTime - m_prevTime) * m_secondsPerCount;
	m_prevTime = m_currTime;
	m_FPStimer += m_deltaTime;
	m_FPScounter++;
	if (m_FPStimer >= 1.0)
	{
		m_FPStimer -= 1.0;
		m_FPS = m_FPScounter;
		m_FPScounter = 0;
	}
}

float Timer::getDelta()
{
	return (float)m_deltaTime;
}

float Timer::getFPS()
{
	return m_FPS;
}
