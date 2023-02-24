#pragma once
#include <windows.h>
#include <assert.h>
#include <inttypes.h>


namespace Engine 
{
namespace Time 
{

typedef uint64_t tick;
CONST int STANDARD_FRAME_RATE = 60;
CONST float STANDARD_FRAME_TIME_SEC = (1.0f / STANDARD_FRAME_RATE);
CONST float MAX_FRAME_TIME_SEC = (2 * STANDARD_FRAME_TIME_SEC);


class Timer
{
private:
	double deltaTime;


public:
	static double DeltaTime;
	LARGE_INTEGER frequency;
	tick lastFrameTick;


	Timer();
	~Timer();

	void Initialize();

	tick GetCurrentTickCounter();
	double GetTimeDiffInSecond(tick startTime, tick endTime);
	double GetTimeDiffInMillisecond(tick startTime, tick endTime);

	double CalculateLastFrameTime();

	double ConvertToMilliSecond(double second);

	double GetDeltaTime();
	void UpdateDeltaTime();


};

}
}

