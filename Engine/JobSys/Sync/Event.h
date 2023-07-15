#pragma once
#include "./WaitableObject.h"

namespace Engine
{

/**
 *	@brief A event object is a synchronization object whose state can be explicitly set to 
 *		   signaled by use of the "Signal()" function. A event object can either be a manual-reset
 *		   object or be an auto-reset object.
 * 
 *		   The event object is useful in sending a signal to a thread notifying the occurrence 
 *		   of an event.
 * 
 *		   This class is a abstract class. It serves as an API contract of its derived classes: 
 *		   "ManualResetEvent" and "AutoResetEvent".
 * 
 *		   See https://learn.microsoft.com/en-us/windows/win32/sync/event-objects for more detail.
 */
class Event : public WaitableObject
{
public:
	virtual bool Wait(WaitTime waitMS) = 0;
	virtual void Signal() = 0;
};


/**
 *	@brief This class creates or opens a named or unnamed event object.
 *	   
 *		   Two or more processes can create the same named event. The first process actually 
 *		   creates the event, and subsequent processes with sufficient access rights simply 
 *		   open a handle to the existing event. Note that the "initiallySignaled" parameter
 *		   of the subsequent processes will be ignored because it have already been set by 
 *		   the creating process.
 * 
 *		   The initial state of the event object is specified by the "initiallySignaled" 
 *		   parameter. All threads have the access to set or reset the event.
 * 
 *		   When the state of a manual-reset event object is signaled, it remains signaled 
 *		   until it is explicitly reset to nonsignaled by the reset function. Any number of 
 *		   waiting threads, or threads that subsequently begin wait operations for the 
 *		   specified event object, can be released while the object's state is signaled.
 */
class ManualResetEvent : public Event
{
public:
	/*	@brief If the input parameter "initiallySignaled" is set to true, the created event
	 *		   will be in a signaled state by default.  
	 *		   In the case of creating a named event and an object already exists prior to 
	 *		   this constructor call, the constructor will store a handler to the existing 
	 *		   object instead. */
	ManualResetEvent(bool initiallySignaled = false, const char* name = nullptr);
	~ManualResetEvent();

	ManualResetEvent(const ManualResetEvent& other) = delete;
	ManualResetEvent& operator=(const ManualResetEvent& other) = delete;

	/*	@brief Reset the event. */
	void Reset();

	/*	@brief Waits until the event is signaled or the time-out interval elapses. Return
	 *		   true if the event is signaled, otherwise return false. */
	bool Wait(WaitTime waitMS = INFINITE);

	/*	@brief Signal the event. */
	void Signal();
};


/**
 *	@brief This class creates or opens a named or unnamed event object.
 *
 *		   Two or more processes can create the same named event. The first process actually
 *		   creates the event, and subsequent processes with sufficient access rights simply
 *		   open a handle to the existing event. Note that the "initiallySignaled" parameter
 *		   of the subsequent processes will be ignored because it have already been set by
 *		   the creating process.
 *
 *		   The initial state of the event object is specified by the "initiallySignaled"
 *		   parameter. All threads have the access to set or reset the event.
 *
 *		   When the state of an auto-reset event object is signaled, it remains signaled 
 *		   until a single waiting thread is released; the system then automatically resets 
 *		   the state to nonsignaled. If no threads are waiting, the event object's state 
 *		   remains signaled.
 */
class AutoResetEvent : public Event
{
public:
	/*	@brief If the input parameter "initiallySignaled" is set to true, the created event
	 *		   will be in a signaled state by default.
	 *		   In the case of creating a named event and an object already exists prior to
	 *		   this constructor call, the constructor will store a handler to the existing
	 *		   object instead. */
	AutoResetEvent(bool initiallySignaled = false, const char* name = nullptr);
	~AutoResetEvent();

	AutoResetEvent(const AutoResetEvent& other) = delete;
	AutoResetEvent& operator=(const AutoResetEvent& other) = delete;

	/*	@brief Waits until the event is signaled or the time-out interval elapses. Return 
	 *		   true if the event is signaled, otherwise return false. */
	bool Wait(WaitTime waitMS = INFINITE);
	
	/*	@brief Signal the event. */
	void Signal();
};


}//Namespace Engine