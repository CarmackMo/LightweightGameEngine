// Includes
//=========

#include "../cMutex_recursive.h"

// Interface
//==========

void eae6320::Concurrency::cMutex_recursive::Lock()
{
	EnterCriticalSection( &m_criticalSection );
}

eae6320::cResult eae6320::Concurrency::cMutex_recursive::LockIfPossible()
{
	return ( TryEnterCriticalSection( &m_criticalSection ) != FALSE ) ? Results::Success : Results::Failure;
}

void eae6320::Concurrency::cMutex_recursive::Unlock()
{
	LeaveCriticalSection( &m_criticalSection );
}

// Initialize / Clean Up
//----------------------

eae6320::Concurrency::cMutex_recursive::cMutex_recursive()
{
	InitializeCriticalSection( &m_criticalSection );
}

eae6320::Concurrency::cMutex_recursive::~cMutex_recursive()
{
	DeleteCriticalSection( &m_criticalSection );
}
