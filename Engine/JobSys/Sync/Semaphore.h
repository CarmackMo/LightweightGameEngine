#pragma once
#include <cassert>
#include "WaitableObject.h"

namespace Engine
{

/**
 *	@brief A semaphore object is a synchronization object that maintains a count between zero and 
 *		   a specified maximum value. The count is decremented each time a thread completes a wait
 *		   for the semaphore object and incremented each time a thread releases the semaphore. 
 *		   When the count reaches zero, no more threads can successfully wait for the semaphore 
 *		   object state to become signaled. The state of a semaphore is set to signaled when its 
 *		   count is greater than zero, and nonsignaled when its count is zero.
 * 
 *		   The semaphore object is useful in controlling a shared resource that can support a 
 *		   limited number of users. It acts as a gate that limits the number of threads sharing 
 *		   the resource to a specified maximum number. When becoming available, if more than one 
 *		   thread is waiting on the semaphore, a waiting thread is selected. Do not assume a 
 *		   first-in, first-out (FIFO) order.
 * 
 *		   If the name of the new semaphore object matches the name of an existing named semaphore 
 *		   object, the calling thread with sufficient access rights simply open a handle to the 
 *		   existing mutex. If the name of the new semaphore matches the name of an existing event, 
 *		   mutex, waitable timer, job, or file-mapping object, the creation fails. This occurs 
 *		   because these objects share the same namespace.
 * 
 *		   Note that a thread that owns a mutex object can wait repeatedly for the same mutex 
 *		   object to become signaled without its execution becoming blocked. A thread that waits 
 *		   repeatedly for the same semaphore object, however, decrements the semaphore's count 
 *		   each time a wait operation is completed; the thread is blocked when the count gets to 
 *		   zero. Similarly, only the thread that owns a mutex can successfully call the mutex 
 *		   release function, though any thread can use semaphore release function to increase the 
 *		   count of a semaphore object.
 * 
 *		   See https://learn.microsoft.com/en-us/windows/win32/sync/semaphore-objects for more detail
 */
class Semaphore : public WaitableObject
{
public:
	/* @brief Creates a named or unnamed semaphore object. */
	Semaphore(unsigned int initCount, unsigned int maxCount, const char* name = nullptr)
	{
		assert(initCount <= maxCount);

		/* Maps the input character string to a UTF-16 (wide character) string. Firstly,
		 * calcualte the buffer size, then create the buffer and map the string to buffer. */
		wchar_t* wChars = nullptr;
		int bytesNeeded = MultiByteToWideChar(CP_ACP, 0, name, -1, wChars, -1);
		if (bytesNeeded)
		{
			wChars = new wchar_t[bytesNeeded];
			int bytesNeeded = MultiByteToWideChar(CP_ACP, 0, name, -1, wChars, -1);
		}

		handle = CreateSemaphore(NULL, initCount, maxCount, wChars);
		assert(handle != NULL);
	}

	~Semaphore()
	{
		BOOL result = CloseHandle(handle);
		assert(result == TRUE);
	}

	Semaphore(const Semaphore& other) = delete;
	Semaphore& operator=(const Semaphore& other) = delete;

	/* @brief Release/increases semaphore's count by a specified amount. The count can never be 
	 *		  less than zero or greater than the maximum value. */
	void Increment(unsigned int count = 1)
	{
		BOOL result = ReleaseSemaphore(handle, count, NULL);
		assert(result == TRUE);
	}

	/* @breif The calling thread will wait for the semaphore object until its count is larger than
	 *		  zero. */
	void Decrement()
	{
		DWORD result = WaitForSingleObject(handle, INFINITE);
		assert(result == WAIT_OBJECT_0);
	}

	/* @brief The calling thread will try to wait for the semaphore object. If the count of the
	 *		  semaphore object is larger than zero, this function will return immediately and the
	 *		  calling thread owns the semaphore. Otherwise, the calling thread will wait until 
	 *		  either the the count is larger than zero or the wait time is expired. */
	bool Decrement(WaitTime waitMS)
	{
		DWORD result = WaitForSingleObject(handle, waitMS);
		assert((waitMS == INFINITE && result == WAIT_OBJECT_0) || (result == WAIT_TIMEOUT));

		return result == WAIT_OBJECT_0;
	}
};

}//Namespace Engine