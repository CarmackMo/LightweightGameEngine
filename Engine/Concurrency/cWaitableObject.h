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

#pragma once

// Includes
//=========

#include <Engine/Results/Results.h>

#if defined( EAE6320_PLATFORM_WINDOWS )
	#include <Engine/Windows/Includes.h>
#endif


// Constance
//==================

namespace eae6320
{
namespace Concurrency
{
	/* A 32-bit unsigned integer */
	typedef DWORD WaitTime;
}
}


// Class Declaration
//==================

namespace eae6320
{
namespace Concurrency
{

	class cWaitableObject
	{

		// Interface
		//==========

	public:

		// Initialization / Clean Up
		//--------------------------

		cWaitableObject() : m_handle(INVALID_HANDLE_VALUE) {}
		virtual ~cWaitableObject() {}

#if defined( EAE6320_PLATFORM_WINDOWS )
		HANDLE GetHandle(void) { return m_handle; }
#endif


		// Implementation
		//==========

	private:

		// Initialization / Clean Up
		//--------------------------
		
		cWaitableObject(const cWaitableObject&) = delete;
		cWaitableObject(cWaitableObject&&) = delete;
		cWaitableObject& operator=(const cWaitableObject&) = delete;
		cWaitableObject& operator=(cWaitableObject&&) = delete;


		// Data
		//==========

	protected:
		/* See https://stackoverflow.com/questions/902967/what-is-a-windows-handle for
		 * more detail of windows "HANDLE" */
#if defined( EAE6320_PLATFORM_WINDOWS )
		HANDLE m_handle;
#endif

	};

}
}


