#include "Timer.h"

/****************************
* Default Constructor
****************************/
Timer::Timer():
	FPS(0),
	avgFPS(0)
{
	startFrame.QuadPart=0;
	if(!QueryPerformanceFrequency(&freq))
		freq.QuadPart=0;	//freq INVALID

	QueryPerformanceCounter(&absoluteStart);
}

/****************************
* Deconstructor
****************************/
Timer::~Timer()
{
	while(!avgFPSdata.empty())
	{
		avgFPSdata.pop();
	}
}

/****************************
* update - This function is called on the beginning of the game
			loop to determine the values of time
****************************/
void Timer::update()
{
	if(startFrame.QuadPart==0)		//initial state
	{
		QueryPerformanceCounter(&startFrame);
		endFrame=startFrame;
	}
	else
	{
		startFrame=endFrame;
		QueryPerformanceCounter(&endFrame);
		
		//calculates the FPS values
		calcFPS();
		calcAvgFPS();
	}

}

/****************************
* getFPS - Returns the frames per second
****************************/
float Timer::getFPS()
{
	return FPS;
}

/****************************
* getAvgFPS - Returns the average frames per second
				for the last 30 frames
****************************/
float Timer::getAvgFPS()
{
	return avgFPS;
}

/****************************
* getMSPF -	Returns the number of milliseconds the last
			frame took
****************************/
float Timer::getMillisecPerFrame()
{
	if(FPS>0)		//sanity check
	{
		return 1000/FPS;
	}
	else
		return 0;
}

/****************************
* getStart - Returns the time value of the beginning of
			the last frame
****************************/
LARGE_INTEGER Timer::getStart()
{
	return startFrame;
}

/****************************
* getEnd -	Returns the time value of the beginning of
			the current frame
****************************/
LARGE_INTEGER Timer::getEnd()
{
	return endFrame;
}

/****************************
* getFreq - Returns the frequency of the cpu clock
****************************/
LARGE_INTEGER Timer::getFreq()
{
	return freq;
}

/****************************
* calcFPS -	Private function used to calculate the frames per second
			for the last frame
****************************/
void Timer::calcFPS()
{
	double span=double(endFrame.QuadPart - startFrame.QuadPart);
	double fr=double(freq.QuadPart);
	double time=span/fr;
	FPS=1/time;
}

/****************************
* calcAvgFPS -	Private function used to calculate the average FPS
				for the last 30 frames
****************************/
void Timer::calcAvgFPS()
{
	float temp=0;
	std::queue<float> temp2;

	//adds the FPS data to the avg data variable - capped at last 30 frames
	if(avgFPSdata.size()<30)
	{
		avgFPSdata.push(FPS);
	}
	else
	{
		avgFPSdata.pop();
		avgFPSdata.push(FPS);
	}
	temp2=avgFPSdata;

	//totals the FPS data
	for(int i=0;i<avgFPSdata.size();i++)
	{
		temp=temp+temp2.front();
		temp2.pop();
	}

	//takes the average of the FPS data
	avgFPS=temp/avgFPSdata.size();
}
