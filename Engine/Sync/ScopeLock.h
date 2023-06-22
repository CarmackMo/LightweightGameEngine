#pragma once
#include "./Sync/WaitableObject.h"

namespace Engine
{

class Mutex : public WaitableObject
{
public:
	Mutex(bool i_bTakeOwnership = false, const char* i_pName = nullptr);
	~Mutex();

	bool TryAcquire();
	void Acquire();
	bool Acquire(WaitTime i_WaitMilliseconds);
	void Release();

private:
	Mutex(const Mutex&) = delete;
	Mutex& operator=(const Mutex&) = delete;
};


class ScopeLock
{
public:
	ScopeLock(Mutex& i_Mutex) :
		m_pMutex(&i_Mutex)
	{
		m_pMutex->Acquire();
	}
	~ScopeLock()
	{
		m_pMutex->Release();
	}

private:
	ScopeLock(const ScopeLock&) = delete;
	ScopeLock& operator=(const ScopeLock&) = delete;

	Mutex* m_pMutex;
};

}//Namespace Engine