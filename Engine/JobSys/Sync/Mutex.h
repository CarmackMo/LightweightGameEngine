#pragma once
#include <cassert>
#include "./WaitableObject.h"

namespace Engine
{

/**
 *	@brief This class creates or opens a named or unnamed mutex object.
 *		   
 *		   Two or more processes can create the same named mutex. The first process actually 
 *		   creates the mutex, and subsequent processes with sufficient access rights simply 
 *		   open a handle to the existing mutex.
 *		   
 *		   The state of a mutex object is signaled when it is not owned by any thread. The 
 *		   creating thread can use the "takeOwnership" flag to request initial ownership of 
 *		   the mutex. Otherwise, a thread must use one of the wait functions to request 
 *		   ownership. When the mutex's state is signaled, one waiting thread is granted 
 *		   ownership, the mutex's state changes to nonsignaled, and the wait function returns. 
 *		   Only one thread can own a mutex at any given time.
 */
class Mutex : public WaitableObject
{
public:
	 /*	@brief If the input parameter "takeOwnership" is set to true and the caller 
	  *		   created the mutex, the calling thread obtains initial ownership of 
	  *		   this mutex.
	  *		   In the case of creating a same-named mutex that already exists 
	  *		   prior to this constructor call, the constructor will open a handler 
	  *		   to the existing mutex instead. */
	inline Mutex(bool takeOwnership = false, const char* name = nullptr)
	{
		/* Maps the input character string to a UTF-16 (wide character) string. Firstly,
		 * calcualte the buffer size, then create the buffer and map the string to buffer. */
		wchar_t* wChars = nullptr;
		int bytesNeeded = MultiByteToWideChar(CP_ACP, 0, name, -1, wChars, -1);
		if (bytesNeeded)
		{
			wChars = new wchar_t[bytesNeeded];
			int bytesNeeded = MultiByteToWideChar(CP_ACP, 0, name, -1, wChars, -1);
		}

		handle = CreateMutex(NULL, (BOOL)takeOwnership, wChars);

		assert(handle != NULL);
	}
	
	inline ~Mutex()
	{
		BOOL result = CloseHandle(handle);
		assert(result == TRUE);
	}

	Mutex(const Mutex& other) = delete;
	Mutex& operator=(const Mutex& other) = delete;

	/*	@brief Check if the mutex can be acquired. */
	bool CanAcquire()
	{
		/* Return "WAIT_OBJECT_0" if the mutex is acquired by the calling thread
		 * Return "WAIT_TIMEOUT"	*/
		DWORD result = WaitForSingleObject(handle, 0);
		return result == WAIT_OBJECT_0;
	}

	/* @brief Try to acquire the mutex ownership within "waitMS" interval. */
	DWORD Acquire(WaitTime waitMS = INFINITE)
	{
		/* The wait function will wait for the mutex state to be set to signaled (released) 
		 * for a maximum time-out duration. Then, it will set the mutex state to nonsignaled
		 * (acquried) immediately and return.
		 * Return "WAIT_OBJECT_0" if the mutex is signaled within the time-out interval.
		 * Return "WAIT_TIMEOUT" if the waiting time exceeds the time-out interval without
		 *		  the mutex being signaled.
		 * Return "WAIT_ABANDONED" if the function does not exceeds the time-out interval,
		 *		  but the thread owning the mutex terminates without releasing the mutex.
		 *		  The calling thread will own the mutex and the mutex state will be set
		 *		  to nonsignaled. */
		return WaitForSingleObject(handle, waitMS);
	}

	/* @brief Release mutex ownership, and set the mutex state to signaled. */
	void Release()
	{
		BOOL result = ReleaseMutex(handle);
		assert(result == TRUE);
	}
};


}//Namespace Engine 
