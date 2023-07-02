#pragma once
#include <cassert>
#include <Windows.h>
#include <inttypes.h>
#include "Utility/Singleton.h"

#define CONSTANT_TRAME_TIME

namespace Engine
{
namespace Timer
{

typedef uint64_t tick;
CONST int STANDARD_FRAME_RATE = 60;
CONST float STANDARD_FRAME_TIME_SEC = (1.0f / STANDARD_FRAME_RATE);
CONST float MAX_FRAME_TIME_SEC = (2 * STANDARD_FRAME_TIME_SEC);


class Time : public Singleton<Time>
{
private:
	LARGE_INTEGER frequency;
	tick lastFrameTick;

	inline Time() :
		frequency({ 0 }),
		lastFrameTick(0)
	{
		Initialize();
	}
	inline ~Time() = default;

public:

	friend class Singleton<Time>;

	static double DeltaTime;

	inline void Initialize()
	{
		BOOL success = QueryPerformanceFrequency(&frequency);
		assert(success == TRUE);
		assert(frequency.QuadPart != 0);
	}

	inline tick GetCurrentTickCounter()
	{
		if (frequency.QuadPart == 0)
			Initialize();

		LARGE_INTEGER tick;
		BOOL success = QueryPerformanceCounter(&tick);
		assert(success == TRUE);
		return tick.QuadPart;
	}
	inline double GetTimeDiffInSecond(tick startTime, tick endTime)
	{
		if (frequency.QuadPart == 0)
			Initialize();

		return static_cast<double>(endTime - startTime) / frequency.QuadPart;
	}
	inline double GetTimeDiffInMillisecond(tick startTime, tick endTime)
	{
		return GetTimeDiffInSecond(startTime, endTime) * 1000.0;
	}

	inline double CalculateLastFrameTime()
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

	inline double ConvertToMilliSecond(double second)
	{
		return second * 1000.0;
	}

	inline double GetDeltaTime()
	{
		return DeltaTime;
	}
	inline void UpdateDeltaTime()
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

};

}
}

