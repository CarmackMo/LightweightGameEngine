#pragma once
#include "./WaitableObject.h"

namespace Engine
{

class Event : public WaitableObject
{
public:
	virtual bool Wait(wait_t i_WaitMilliseconds) = 0;
	virtual void Signal() = 0;

};

class ManualResetEvent : public Event
{
public:
	ManualResetEvent(bool i_bInitiallySignaled = false, const char* i_pName = nullptr);
	~ManualResetEvent();

	ManualResetEvent(const ManualResetEvent&) = delete;
	ManualResetEvent& operator=(const ManualResetEvent&) = delete;

	void Reset();

	bool Wait(wait_t i_WaitMilliseconds = WaitInfinite);
	void Signal();
};

class AutoResetEvent : public Event
{
public:
	AutoResetEvent(bool i_bInitiallySignaled = false, const char* i_pName = nullptr);
	~AutoResetEvent();

	AutoResetEvent(const AutoResetEvent&) = delete;
	AutoResetEvent& operator=(const AutoResetEvent&) = delete;

	bool Wait(wait_t i_WaitMilliseconds = INFINITE);
	void Signal();
};

}//Namespace Engine