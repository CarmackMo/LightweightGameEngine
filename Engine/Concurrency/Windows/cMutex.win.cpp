// Includes
//=========

#include <Engine/Asserts/Asserts.h>
#include <Engine/Concurrency/cMutex.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Windows/Functions.h>


// Interface
//==========

bool eae6320::Concurrency::cMutex::CanAcquire()
{
	/* Return "WAIT_OBJECT_0" if the mutex is acquired by the calling thread
	 * Return "WAIT_TIMEOUT" if the mutex is acquired by other thead.	*/
	DWORD result = WaitForSingleObject(m_handle, 0);
	return result == WAIT_OBJECT_0;
}


DWORD eae6320::Concurrency::cMutex::Acquire(eae6320::Concurrency::WaitTime waitMS)
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
	return WaitForSingleObject(m_handle, waitMS);
}


/* @brief Release mutex ownership, and set the mutex state to signaled. */
void eae6320::Concurrency::cMutex::Release()
{
	BOOL result = ReleaseMutex(m_handle);
	EAE6320_ASSERT(result == TRUE);
}


// Initialize / Clean Up
//----------------------


eae6320::cResult eae6320::Concurrency::cMutex::Initialize(bool takeOwnership, const char* name)
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

	if (m_handle = CreateMutex(NULL, (BOOL)takeOwnership, wChars))
	{
		return Results::Success;
	}
	else
	{
		const auto errorMessage = Windows::GetLastSystemError();
		EAE6320_ASSERTF(false, "Couldn't create mutex: %s", errorMessage.c_str());
		Logging::OutputError("Windows failed to create a mutex: %s", errorMessage.c_str());
		return Results::Failure;
	}
}


eae6320::cResult eae6320::Concurrency::cMutex::CleanUp()
{
	if (m_handle)
	{
		if (CloseHandle(m_handle) == FALSE)
		{
			const auto errorMessage = Windows::GetLastSystemError();
			EAE6320_ASSERTF(false, "Couldn't close mutex handle: %s", errorMessage.c_str());
			Logging::OutputError("Windows failed to close a mutex handle: %s", errorMessage.c_str());
		}
		m_handle = NULL;
	}

	return Results::Success;
}


eae6320::Concurrency::cMutex::cMutex()
{

}


eae6320::Concurrency::cMutex::~cMutex()
{
	const auto result = CleanUp();
	EAE6320_ASSERT(result);
}