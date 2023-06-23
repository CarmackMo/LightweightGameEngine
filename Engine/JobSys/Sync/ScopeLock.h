#pragma once
#include "./Mutex.h"

namespace Engine
{

class ScopeLock
{
private:
	Mutex* mutex;

public:
	ScopeLock(Mutex& mutex) : mutex(&mutex)
	{
		this->mutex->Acquire();
	}
	
	~ScopeLock()
	{
		mutex->Release();
	}

	ScopeLock(const ScopeLock&) = delete;
	ScopeLock& operator=(const ScopeLock&) = delete;
};

}//Namespace Engine