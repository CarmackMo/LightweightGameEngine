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

#pragma once

// Includes
//=========

#include "Constants.h"

#include <Engine/Concurrency/cWaitableObject.h>
#include <Engine/Results/Results.h>

#if defined( EAE6320_PLATFORM_WINDOWS )
	#include <Engine/Windows/Includes.h>
#endif


namespace eae6320
{
namespace Concurrency
{

	class cSemaphore : public cWaitableObject
	{

	public:

		// Interface
		//=====================

	public:

		/* @brief Release/increases semaphore's count by a specified amount. The count can never be
		 *		  less than zero or greater than the maximum value. */
		void Increment(unsigned int count = 1);

		/* @breif The calling thread will wait for the semaphore object until its count is larger than
		 *		  zero. */
		void Decrement();

		/* @brief The calling thread will try to wait for the semaphore object. If the count of the
		 *		  semaphore object is larger than zero, this function will return immediately and the
		 *		  calling thread owns the semaphore. Otherwise, the calling thread will wait until
		 *		  either the the count is larger than zero or the wait time is expired. */
		bool Decrement(WaitTime waitMS);

		// Initialization / Clean Up
		//--------------------------

		cResult Initialize(unsigned int initCount, unsigned int maxCount, const char* name = nullptr);
		cResult CleanUp();

		cSemaphore();
		~cSemaphore();


		// Implementation
		//=====================

	private:

		// Initialization / Clean Up
		//--------------------------

		cSemaphore(const cSemaphore& other) = delete;
		cSemaphore(cSemaphore&& other) = delete;
		cSemaphore& operator=(const cSemaphore& other) = delete;
		cSemaphore& operator=(cSemaphore&& other) = delete;

	};


}// Namespace Concurrency
}// Namespace eae6320