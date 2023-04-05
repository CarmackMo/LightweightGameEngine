#pragma once
#pragma once
#include "Dependency.h"
#include "DataType.h"

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

	inline Time();
	inline ~Time();

public:

	friend class Singleton<Time>;

	static double DeltaTime;

	inline void Initialize();

	inline tick GetCurrentTickCounter();
	inline double GetTimeDiffInSecond(tick startTime, tick endTime);
	inline double GetTimeDiffInMillisecond(tick startTime, tick endTime);

	inline double CalculateLastFrameTime();

	inline double ConvertToMilliSecond(double second);

	inline double GetDeltaTime();
	inline void UpdateDeltaTime();


};

#include "Timer.inl"

}
}

