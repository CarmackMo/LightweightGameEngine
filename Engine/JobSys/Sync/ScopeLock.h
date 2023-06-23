#pragma once
#include "./Mutex.h"

namespace Engine
{

class ScopeLock
{
private:
	Mutex* mutex;

public:
	ScopeLock(Mutex& i_Mutex) : mutex(&i_Mutex)
	{
		mutex->Acquire();
	}
	
	~ScopeLock()
	{
		mutex->Release();
	}

	ScopeLock(const ScopeLock&) = delete;
	ScopeLock& operator=(const ScopeLock&) = delete;
};

}//Namespace Engine