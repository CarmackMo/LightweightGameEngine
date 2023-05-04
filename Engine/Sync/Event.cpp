#include "Event.h"


ManualResetEvent::ManualResetEvent(bool i_bInitiallySignaled, const char* i_pName)
{
	wchar_t* wChars = nullptr;
	int BytesNeeded = MultiByteToWideChar(CP_ACP, 0, i_pName, -1, wChars, -1);
	if (BytesNeeded)
	{
		wChars = new wchar_t[BytesNeeded];
		int BytesNeeded = MultiByteToWideChar(CP_ACP, 0, i_pName, -1, wChars, -1);
	}

	m_Handle = CreateEvent(NULL, TRUE, i_bInitiallySignaled, wChars);

	assert(m_Handle != INVALID_HANDLE_VALUE);
}

ManualResetEvent::~ManualResetEvent()
{
	CloseHandle(m_Handle);
}

void ManualResetEvent::Reset(void)
{
	ResetEvent(m_Handle);
}

bool ManualResetEvent::Wait(wait_t i_WaitMilliseconds)
{
	DWORD result = WaitForSingleObject(m_Handle, i_WaitMilliseconds);
	assert(((i_WaitMilliseconds == WaitInfinite) && (result == WAIT_OBJECT_0)) || (result == WAIT_TIMEOUT));

	return result == WAIT_OBJECT_0;
}

void ManualResetEvent::Signal(void)
{
	BOOL result = SetEvent(m_Handle);
	assert(result == TRUE);
}

AutoResetEvent::AutoResetEvent(bool i_bInitiallySignaled, const char* i_pName)
{
	wchar_t* wChars = nullptr;
	int BytesNeeded = MultiByteToWideChar(CP_ACP, 0, i_pName, -1, wChars, -1);
	if (BytesNeeded)
	{
		wChars = new wchar_t[BytesNeeded];
		int BytesNeeded = MultiByteToWideChar(CP_ACP, 0, i_pName, -1, wChars, -1);
	}

	m_Handle = CreateEvent(NULL, FALSE, i_bInitiallySignaled, wChars);

	assert(m_Handle != INVALID_HANDLE_VALUE);
}

AutoResetEvent::~AutoResetEvent()
{
	CloseHandle(m_Handle);
}

bool AutoResetEvent::Wait(wait_t i_WaitMilliseconds)
{
	DWORD result = WaitForSingleObject(m_Handle, i_WaitMilliseconds);
	assert((result == WAIT_OBJECT_0) || (result == WAIT_TIMEOUT && i_WaitMilliseconds != WaitInfinite));

	return result == WAIT_OBJECT_0;
}

void AutoResetEvent::Signal(void)
{
	BOOL result = SetEvent(m_Handle);
	assert(result == TRUE);
}
