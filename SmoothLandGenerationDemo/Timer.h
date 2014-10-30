#ifndef Timer_H
#define Timer_H

#include <Windows.h>
#include <queue>


class Timer
{
public:
	Timer();
	~Timer();
	void update();
	float getFPS();
	float getAvgFPS();
	float getMillisecPerFrame();
	LARGE_INTEGER getStart();
	LARGE_INTEGER getEnd();
	LARGE_INTEGER getFreq();

private:
	LARGE_INTEGER startFrame;
	LARGE_INTEGER endFrame;
	LARGE_INTEGER freq;
	LARGE_INTEGER absoluteStart;
	float FPS;
	float avgFPS;
	std::queue<float> avgFPSdata;

	void calcFPS();
	void calcAvgFPS();
};

#endif