#pragma once
#include <Windows.h>

namespace Engine
{

class WaitableObject
{
protected:
	/* A handel to an event object.
	 * See https://stackoverflow.com/questions/902967/what-is-a-windows-handle for 
	 * more detail of windows "HANDLE" */
	HANDLE handle;

public:
	/* A 32-bit unsigned integer */
	typedef DWORD WaitTime;

	WaitableObject() : handle(INVALID_HANDLE_VALUE) {}
	virtual ~WaitableObject() {}

	WaitableObject(const WaitableObject& i_other) = delete;
	WaitableObject& operator=(const WaitableObject& i_other) = delete;

	HANDLE GetHandle(void) { return handle; }
};


}//Namespace Engine
