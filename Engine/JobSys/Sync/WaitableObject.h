#pragma once
#include <Windows.h>

namespace Engine
{

/** 
 *	@brief This class encapsulaties Windows APIs for synchronizable objects such as Events, Threads,
 *		   Mutexes. It serves as an API contract for all objects in the game engine that are 
 *		   intended to be thread-waitable and synchronizable.
 * 
 *		   Instances of this class maintain a handler to a Windows synchronizable object. 
 *		   It is the user's responsibility to design a customized constructor and destructor to 
 *		   properly manage the lifecycle of the handler, ensuring proper initialization and 
 *		   cleanup as needed.
 */
class WaitableObject
{
protected:
	/* See https://stackoverflow.com/questions/902967/what-is-a-windows-handle for 
	 * more detail of windows "HANDLE" */
	HANDLE handle;

public:
	/* A 32-bit unsigned integer */
	typedef DWORD WaitTime;

	WaitableObject() : handle(INVALID_HANDLE_VALUE) {}
	virtual ~WaitableObject() {}

	WaitableObject(const WaitableObject& other) = delete;
	WaitableObject& operator=(const WaitableObject& other) = delete;

	HANDLE GetHandle(void) { return handle; }
};


}//Namespace Engine
