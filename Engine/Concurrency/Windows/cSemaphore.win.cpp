// Includes
//=========

#include <Engine/Asserts/Asserts.h>
#include <Engine/Concurrency/cSemaphore.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Windows/Functions.h>


eae6320::cResult eae6320::Concurrency::cSemaphore::Initialize(unsigned int initCount, unsigned int maxCount, const char* name)
{
	EAE6320_ASSERT(initCount <= maxCount);

	/* Maps the input character string to a UTF-16 (wide character) string. Firstly,
	 * calcualte the buffer size, then create the buffer and map the string to buffer. */
	wchar_t* wChars = nullptr;
	int bytesNeeded = MultiByteToWideChar(CP_ACP, 0, name, -1, wChars, -1);
	if (bytesNeeded)
	{
		wChars = new wchar_t[bytesNeeded];
		int bytesNeeded = MultiByteToWideChar(CP_ACP, 0, name, -1, wChars, -1);
	}

	if (m_handle = CreateSemaphore(NULL, initCount, maxCount, wChars))
	{
		return Results::Success;
	}
	else
	{
		const auto errorMessage = Windows::GetLastSystemError();
		EAE6320_ASSERTF(false, "Couldn't create semaphore: %s", errorMessage.c_str());
		Logging::OutputError("Windows failed to create an semaphore: %s", errorMessage.c_str());
		return Results::Failure;
	}
}


eae6320::cResult eae6320::Concurrency::cSemaphore::CleanUp()
{
	if (m_handle)
	{
		if (CloseHandle(m_handle) == FALSE)
		{
			const auto errorMessage = Windows::GetLastSystemError();
			EAE6320_ASSERTF(false, "Couldn't close semaphore handle: %s", errorMessage.c_str());
			Logging::OutputError("Windows failed to close an semaphore handle: %s", errorMessage.c_str());
		}
		m_handle = NULL;
	}

	return Results::Success;
}


eae6320::Concurrency::cSemaphore::cSemaphore() : 
	m_handle(INVALID_HANDLE_VALUE) 
{}


eae6320::Concurrency::cSemaphore::~cSemaphore()
{
	const auto result = CleanUp();
	EAE6320_ASSERT(result);
}


HANDLE eae6320::Concurrency::cSemaphore::GetHandle(void)
{
	return m_handle;
}


void eae6320::Concurrency::cSemaphore::Increment(unsigned int count)
{
	BOOL result = ReleaseSemaphore(m_handle, count, NULL);
	EAE6320_ASSERT(result == TRUE);
}


void eae6320::Concurrency::cSemaphore::Decrement()
{
	DWORD result = WaitForSingleObject(m_handle, INFINITE);
	EAE6320_ASSERT(result == WAIT_OBJECT_0);
}


bool eae6320::Concurrency::cSemaphore::Decrement(WaitTime waitMS)
{
	DWORD result = WaitForSingleObject(m_handle, waitMS);
	EAE6320_ASSERT((waitMS == INFINITE && result == WAIT_OBJECT_0) || (result == WAIT_TIMEOUT));

	return result == WAIT_OBJECT_0;
}