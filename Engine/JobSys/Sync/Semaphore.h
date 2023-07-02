#pragma once
#include <cassert>
#include "WaitableObject.h"

namespace Engine
{

/**
 *	@brief This class creates a named or unnamed semaphore object.
 * 
 *		   If the name of the new semaphore object matches the name of an existing named semaphore 
 *		   object, the calling thread with sufficient access rights simply open a handle to the 
 *		   existing mutex. If the name of the new semaphore matches the name of an existing event, 
 *		   mutex, waitable timer, job, or file-mapping object, the creation fails. This occurs 
 *		   because these objects share the same namespace.
 * 
 *		   The state of a semaphore object is signaled when its count is greater than zero, and 
 *		   nonsignaled when its count is equal to zero. The count can never be less than zero or 
 *		   greater than the specified max value.
 */
class Semaphore : public WaitableObject
{
public:
	Semaphore(unsigned int initCount, unsigned int maxCount, const char* name = nullptr)
	{
		assert(initCount <= maxCount);

		wchar_t* wChars = nullptr;

		/* Maps the input character string to a UTF-16 (wide character) string. Firstly,
		 * calcualte the buffer size, then create the buffer and map the string to buffer. */
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

	Semaphore(const Semaphore&) = delete;
	Semaphore& operator=(const Semaphore&) = delete;

	void Increment(unsigned int count = 1)
	{
		BOOL result = ReleaseSemaphore(handle, count, NULL);
		assert(result == TRUE);
	}

	void Decrement()
	{
		DWORD result = WaitForSingleObject(handle, INFINITE);
		assert(result == WAIT_OBJECT_0);
	}

	bool Decrement(WaitTime waitMS)
	{
		DWORD result = WaitForSingleObject(handle, waitMS);
		assert((waitMS == INFINITE && result == WAIT_OBJECT_0) || (result == WAIT_TIMEOUT));

		return result == WAIT_OBJECT_0;
	}
};

}//Namespace Engine