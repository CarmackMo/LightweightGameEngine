#include <cassert>
#include "Event.h"

namespace Engine
{

#pragma region ManualResetEvent

ManualResetEvent::ManualResetEvent(bool initiallySignaled, const char* name)
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

	handle = CreateEvent(NULL, TRUE, initiallySignaled, wChars);

	assert(handle != INVALID_HANDLE_VALUE);
}


ManualResetEvent::~ManualResetEvent()
{
	CloseHandle(handle);
}


void ManualResetEvent::Reset(void)
{
	ResetEvent(handle);
}


bool ManualResetEvent::Wait(WaitTime waitMS)
{
	/* Return "WAIT_OBJECT_0" if the event is signaled, return "WAIT_TIMEOUT" if the 
	 * time-out interval elapses. */
	DWORD result = WaitForSingleObject(handle, waitMS);
	assert((waitMS == INFINITE && result == WAIT_OBJECT_0) || (result == WAIT_TIMEOUT));

	return result == WAIT_OBJECT_0;
}


void ManualResetEvent::Signal(void)
{
	BOOL result = SetEvent(handle);
	assert(result == TRUE);
}

#pragma endregion


#pragma region AutoResetEvent

AutoResetEvent::AutoResetEvent(bool initiallySignaled, const char* name)
{
	/* Maps the input character string to a UTF-16 (wide character) string. Firstly,
	 * calcualte the buffer size, then create the buffer and map the string to buffer. */
	wchar_t* wChars = nullptr;
	int bytesNeeded = MultiByteToWideChar(CP_ACP, 0, name, -1, wChars, -1);
	if (bytesNeeded)
	{
		wChars = new wchar_t[bytesNeeded];
		int BytesNeeded = MultiByteToWideChar(CP_ACP, 0, name, -1, wChars, -1);
	}

	handle = CreateEvent(NULL, FALSE, initiallySignaled, wChars);

	assert(handle != INVALID_HANDLE_VALUE);
}


AutoResetEvent::~AutoResetEvent()
{
	CloseHandle(handle);
}


bool AutoResetEvent::Wait(WaitTime waitMS)
{
	/* Return "WAIT_OBJECT_0" if the event is signaled, return "WAIT_TIMEOUT" if the
	 * time-out interval elapses. */
	DWORD result = WaitForSingleObject(handle, waitMS);
	assert((result == WAIT_OBJECT_0) || (result == WAIT_TIMEOUT && waitMS != INFINITE));

	return result == WAIT_OBJECT_0;
}


void AutoResetEvent::Signal(void)
{
	BOOL result = SetEvent(handle);
	assert(result == TRUE);
}

#pragma endregion

}//Namespace Engine