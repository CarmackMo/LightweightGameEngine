#pragma once

inline Time::Time() :
	frequency({ 0 }),
	lastFrameTick(0)
{
	Initialize();
}

inline Time::~Time()
{ }

inline void Time::Initialize()
{
	BOOL success = QueryPerformanceFrequency(&frequency);
	assert(success == TRUE);
	assert(frequency.QuadPart != 0);
}

inline tick Time::GetCurrentTickCounter()
{
	if (frequency.QuadPart == 0)
		Initialize();

	LARGE_INTEGER tick;
	BOOL success = QueryPerformanceCounter(&tick);
	assert(success == TRUE);
	return tick.QuadPart;
}

inline double Time::GetTimeDiffInSecond(tick startTime, tick endTime)
{
	if (frequency.QuadPart == 0)
		Initialize();

	return static_cast<double>(endTime - startTime) / frequency.QuadPart;
}

inline double Time::GetTimeDiffInMillisecond(tick startTime, tick endTime)
{
	return GetTimeDiffInSecond(startTime, endTime) * 1000.0;
}

inline double Time::CalculateLastFrameTime()
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

inline double Time::ConvertToMilliSecond(double second)
{
	return second * 1000.0;
}

inline double Time::GetDeltaTime()
{
	return DeltaTime;
}

inline void Time::UpdateDeltaTime()
{
	double lastFrameTime = CalculateLastFrameTime();

#if defined (CONSTANT_TRAME_TIME)
	DeltaTime = STANDARD_FRAME_TIME_SEC;
#elif defined (CLAMP_FRAME_TIME)
	if (lastFrameTime > MAX_FRAME_TIME_SEC)
		DeltaTime = MAX_FRAME_TIME_SEC;
	else
		DeltaTime = lastFrameTime;
#else
	DeltaTime = lastFrameTime;
#endif
}