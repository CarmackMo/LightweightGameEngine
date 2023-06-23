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
 *		   users to manually reset the event object after it is signaled. Provides 
 *		   functions that manage the event object (reset, signal, set wait).
 */
class ManualResetEvent : public Event
{
public:
	/*	@brief If the input parameter "initiallySignaled" is set to true, the 
	 *		   created event will be in a signaled state by default.  
	 *		   In the case of creating a named event and an object already exists 
	 *		   prior to this constructor call, the constructor will store a handler 
	 *		   to the existing object instead. */
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

	/*	@brief Signal the event. */
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
	/*	@brief If the input parameter "initiallySignaled" is set to true, the
	 *		   created event will be in a signaled state by default.
	 *		   In the case of creating a named event and an object already exists
	 *		   prior to this constructor call, the constructor will store a handler
	 *		   to the existing object instead. */
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