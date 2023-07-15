#pragma once
#include "./Mutex.h"

namespace Engine
{

/**
 *	@brief A scopelock object is a synchronization object that maintains a pointer to a mutex 
 *		   object.
 *
 *		   At the time when a scopelock object is constructed, it attempts to acquire ownership of
 *		   the associated mutex. If the mutex is currently owned by another thread, the 
 *		   construction of the scopelock object will be blocked until the mutex is released. The 
 *		   scopelock holds ownership of the mutex for the duration of its lifetime. Upon 
 *		   destruction of the scopelock object , the mutex is automatically released.  
 *		 
 *		   The scopelock object is useful in preventing threads from generating dead lock. 
 */
class ScopeLock
{
private:
	Mutex* mutex;

public:
	ScopeLock(Mutex* mutex) : mutex(mutex)
	{
		this->mutex->Acquire();
	}
	
	~ScopeLock()
	{
		mutex->Release();
	}

	ScopeLock(const ScopeLock& other) = delete;
	ScopeLock& operator=(const ScopeLock& other) = delete;
};

}//Namespace Engine