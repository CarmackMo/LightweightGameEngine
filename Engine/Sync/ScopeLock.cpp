#include <cassert>
#include "ScopeLock.h"

namespace Engine
{

Mutex::Mutex(bool i_bTakeOwnership, const char* i_pName)
{
	wchar_t* pWChars = nullptr;

	int BytesNeeded = MultiByteToWideChar(CP_ACP, 0, i_pName, -1, pWChars, -1);
	if (BytesNeeded)
	{
		pWChars = new wchar_t[BytesNeeded];
		int BytesNeeded = MultiByteToWideChar(CP_ACP, 0, i_pName, -1, pWChars, -1);
	}

	handle = CreateMutex(NULL, (BOOL)i_bTakeOwnership, pWChars);

	assert(handle != NULL);
}

Mutex::~Mutex()
{
	BOOL result = CloseHandle(handle);
	assert(result == TRUE);
}

bool Mutex::TryAcquire(void)
{
	DWORD result = WaitForSingleObject(handle, 0);

	return result == WAIT_OBJECT_0;
}

void Mutex::Acquire(void)
{
	DWORD result = WaitForSingleObject(handle, INFINITE);
	assert(result == WAIT_OBJECT_0);

}

bool Mutex::Acquire(WaitTime i_WaitMilliseconds)
{
	DWORD result = WaitForSingleObject(handle, i_WaitMilliseconds);
	assert(((i_WaitMilliseconds == INFINITE) && (result == WAIT_OBJECT_0)) || (result == WAIT_TIMEOUT));

	return result == WAIT_OBJECT_0;
}

void Mutex::Release(void)
{
	BOOL result = ReleaseMutex(handle);
	assert(result == TRUE);
}

}//Namespace Engine