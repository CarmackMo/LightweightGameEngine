#include "Time.h"


using namespace Engine::Time;

Timer::Timer()
{
	frequency = { 0 };
	lastFrameTick = 0;
	deltaTime = STANDARD_FRAME_TIME_SEC;
	Initialize();
}

Timer::~Timer()
{ }

void Timer::Initialize()
{
	BOOL success = QueryPerformanceFrequency(&frequency);
	assert(success == TRUE);
	assert(frequency.QuadPart != 0);
}

tick Timer::GetCurrentTickCounter()
{
	if (frequency.QuadPart == 0)
		Initialize();

	LARGE_INTEGER tick;
	BOOL success = QueryPerformanceCounter(&tick);
	assert(success == TRUE);
	return tick.QuadPart;
}

double Timer::GetTimeDiffInSecond(tick startTime, tick endTime)
{
	if (frequency.QuadPart == 0)
		Initialize();

	return static_cast<double>(endTime - startTime) / frequency.QuadPart;
}

double Timer::GetTimeDiffInMillisecond(tick startTime, tick endTime)
{
	return GetTimeDiffInSecond(startTime, endTime) * 1000.0;
}

double Timer::CalculateLastFrameTime()
{
	tick currentTick = GetCurrentTickCounter();
	double lastFrameTime;

	if (lastFrameTick)
		lastFrameTime = GetTimeDiffInSecond(lastFrameTick, currentTick);
	else
		lastFrameTime = STANDARD_FRAME_TIME_SEC;

	lastFrameTick = currentTick;
	return lastFrameTime;
}

double Timer::ConvertToMilliSecond(double second)
{
	return second * 1000.0;
}

double Timer::GetDeltaTime()
{
	return deltaTime;
}

void Timer::UpdateDeltaTime()
{
	double lastFrameTime = CalculateLastFrameTime();

#if defined (CONSTANT_TRAME_TIME)
	deltaTime = STANDARD_FRAME_TIME_SEC;
#elif defined (CLAMP_FRAME_TIME)
	if (lastFrameTime > MAX_FRAME_TIME_SEC)
		deltaTime = MAX_FRAME_TIME_SEC;
	else
		deltaTime = lastFrameTime;
#else
	deltaTime = lastFrameTime;
#endif
}