/*
	A mutex is used to protect resources that are shared between more than one thread

	The name comes from "mutual exclusion".
	A mutex provides a locking mechanism,
	and the mutually-exclusive lock can only be acquired by a single thread at any one time.
*/

#ifndef EAE6320_CONCURRENCY_CMUTEX_H
#define EAE6320_CONCURRENCY_CMUTEX_H

// Includes
//=========

#include <Engine/Results/Results.h>

#if defined( EAE6320_PLATFORM_WINDOWS )
	#include <Engine/Windows/Includes.h>
#endif

// Class Declaration
//==================

namespace eae6320
{
	namespace Concurrency
	{
		class cMutex
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

				cScopeLock( cMutex& io_mutex ) : m_mutex( io_mutex ) { m_mutex.Lock(); }
				~cScopeLock() { m_mutex.Unlock(); }

				cScopeLock( const cScopeLock& ) = delete;
				cScopeLock( cScopeLock&& ) = delete;
				cScopeLock& operator =( const cScopeLock& ) = delete;
				cScopeLock& operator =( cScopeLock&& ) = delete;

			private:

				cMutex& m_mutex;
			};

			// Calling this function will block the thread until the lock is acquired
			void Lock();
			// Calling this function returns immediately.
			// If the return value succeeds then the lock was acquired
			// but if the return value fails then the lock wasn't acquired
			// (because another thread already holds the mutually-exclusive lock)
			cResult LockIfPossible();
			// This releases a held lock.
			// The results are undefined if this is called when a lock isn't held.
			void Unlock();

			// Initialization / Clean Up
			//--------------------------

			cMutex();
			~cMutex();

			// Data
			//=====

		private:

#if defined( EAE6320_PLATFORM_WINDOWS )
			SRWLOCK m_srwLock;
#endif

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
