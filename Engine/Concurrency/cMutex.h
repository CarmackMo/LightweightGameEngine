/**
 *	@brief A mutex object is a synchronization object whose state is set to signaled when it is
 *		   not owned by any thread, and nonsignaled when it is owned. Only one thread at a time
 *		   can own a mutex object.
 *
 *		   The mutex object is useful in coordinating mutually exclusive access to a shared
 *		   resource. Note that critical section objects provide synchronization similar to that
 *		   provided by mutex objects, except that critical section objects can be used only by
 *		   the threads of a single process.
 *
 *		   The creating thread of the mutex can set the "takeOwnership" flag to request initial
 *		   ownership of the mutex. Otherwise, a thread must use the "Acquire()" functions to
 *		   request ownership. If the mutex object is owned by another thread, the "Acquire()"
 *		   function blocks the requesting thread until the owning thread releases the mutex object.
 *		   If more than one thread is waiting on a mutex, a waiting thread is selected. Do not
 *		   assume a first-in, first-out (FIFO) order.
 *
 *		   After a thread obtains ownership of a mutex, it can specify the same mutex in repeated
 *		   calls to the "Acquire()" function without blocking its execution. This prevents a
 *		   thread from deadlocking itself while waiting for a mutex that it already owns.
 *
 *		   Two or more processes can create the same named mutex. The first process actually
 *		   creates the mutex, and subsequent processes with sufficient access rights simply
 *		   open a handle to the existing mutex.
 *
 *		   See https://learn.microsoft.com/en-us/windows/win32/sync/mutex-objects for more detail.
 */

#ifndef EAE6320_CONCURRENCY_CMUTEX_H
#define EAE6320_CONCURRENCY_CMUTEX_H

// Includes
//=========

#include <Engine/Results/Results.h>
#include <Engine/Concurrency/cWaitableObject.h>

#if defined( EAE6320_PLATFORM_WINDOWS )
	#include <Engine/Windows/Includes.h>
#endif

// Class Declaration
//==================

namespace eae6320
{
namespace Concurrency
{
	class cMutex : public cWaitableObject
	{
		// Interface
		//==========

	public:

		// This is a convenience class that automatically handles acquiring and releasing a mutex at scope level
		// (after an instance has been constructed the lock will be acquired,
		// and the lock will automatically be released once the instance goes out of scope and is destructed)
		class cScopeLock
		{
		public:

			cScopeLock( cMutex& io_mutex ) : m_mutex( io_mutex ) { m_mutex.Acquire(); }
			~cScopeLock() { m_mutex.Release(); }

			cScopeLock( const cScopeLock& ) = delete;
			cScopeLock( cScopeLock&& ) = delete;
			cScopeLock& operator =( const cScopeLock& ) = delete;
			cScopeLock& operator =( cScopeLock&& ) = delete;

		private:

			cMutex& m_mutex;
		};


		// Interface
		//==========

	public:

		/*	@brief If the input parameter "takeOwnership" is set to true and the caller
		 *		   created the mutex, the calling thread obtains initial ownership of
		 *		   this mutex.
		 *		   In the case of creating a same-named mutex that already exists
		 *		   prior to this constructor call, the constructor will open a handler
		 *		   to the existing mutex instead. */
		cResult Initialize(bool takeOwnership = false, const char* name = nullptr);
		cResult CleanUp();

		cMutex();
		~cMutex();

		/*	@brief Check if the mutex can be acquired. */
		bool CanAcquire();

		/* @brief Try to acquire the mutex ownership within "waitMS" interval. If the mutex
		 *		  is not available, the calling thread will wait until either the mutex is
		 *		  released or the wait time is expired. */
		DWORD Acquire(WaitTime waitMS = INFINITE);

		/* @brief Release mutex ownership, and set the mutex state to signaled. */
		void Release();


		// Implementation
		//---------------

	private:

		// Initialization / Clean Up
		//--------------------------

		cMutex( const cMutex& ) = delete;
		cMutex( cMutex&& ) = delete;
		cMutex& operator =( const cMutex& ) = delete;
		cMutex& operator =( cMutex&& ) = delete;
	};
}
}

#endif	// EAE6320_CONCURRENCY_CMUTEX_H
