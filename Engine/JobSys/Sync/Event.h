#pragma once
#include "./WaitableObject.h"

namespace Engine
{

/**
 *	@brief A abstract class. It serves as a API contract of its derived classes: 
 *		   "ManualResetEvent" and "AutoResetEvent"
 */
class Event : public WaitableObject
{
public:
	virtual bool Wait(WaitTime waitMS) = 0;
	virtual void Signal() = 0;
};


/**
 *	@brief This class creates or opens a named or unnamed event object that requres
 *		   users tomanually reset the event object after it is signaled. Provides 
 *		   functions that manage the event object (reset, signal, set wait).
 */
class ManualResetEvent : public Event
{
public:
	ManualResetEvent(bool initiallySignaled = false, const char* name = nullptr);
	~ManualResetEvent();

	ManualResetEvent(const ManualResetEvent&) = delete;
	ManualResetEvent& operator=(const ManualResetEvent&) = delete;

	/*	@brief Reset the event. */
	void Reset();

	/*	@brief Waits until the event is in the signaled state or the time-out 
	 *		   interval elapses. Return true if the event is signaled, otherwise 
	 *		   return false. */
	bool Wait(WaitTime waitMS = INFINITE);

	/*	@breif Signal the event. */
	void Signal();
};


/**
 *	@brief This class creates or opens a named or unnamed event object that resets 
 *		   the event state to nonsignaled automatically after a single waiting 
 *		   thread has been released. Provides functions that manage the event object 
 *		   (signal, set wait).
 */
class AutoResetEvent : public Event
{
public:
	AutoResetEvent(bool initiallySignaled = false, const char* name = nullptr);
	~AutoResetEvent();

	AutoResetEvent(const AutoResetEvent&) = delete;
	AutoResetEvent& operator=(const AutoResetEvent&) = delete;

	/*	@brief Waits until the event is in the signaled state or the time-out
	 *		   interval elapses. Return true if the event is signaled, otherwise
	 *		   return false. */
	bool Wait(WaitTime waitMS = INFINITE);
	
	/*	@breif Signal the event. */
	void Signal();
};


}//Namespace Engine