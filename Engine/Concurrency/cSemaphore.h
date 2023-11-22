#pragma once

// Includes
//=========

#include "Constants.h"

#include <Engine/Results/Results.h>

#if defined( EAE6320_PLATFORM_WINDOWS )
	#include <Engine/Windows/Includes.h>
#endif


namespace eae6320
{
namespace Concurrency
{

	class cSemaphore
	{

	public:

		/* A 32-bit unsigned integer */
		typedef DWORD WaitTime;


		// Interface
		//=====================

	public:

		HANDLE GetHandle(void);

		/* @brief Release/increases semaphore's count by a specified amount. The count can never be
		 *		  less than zero or greater than the maximum value. */
		void Increment(unsigned int count = 1);

		/* @breif The calling thread will wait for the semaphore object until its count is larger than
		 *		  zero. */
		void Decrement();

		/* @brief The calling thread will try to wait for the semaphore object. If the count of the
		 *		  semaphore object is larger than zero, this function will return immediately and the
		 *		  calling thread owns the semaphore. Otherwise, the calling thread will wait until
		 *		  either the the count is larger than zero or the wait time is expired. */
		bool Decrement(WaitTime waitMS);

		// Initialization / Clean Up
		//--------------------------

		cResult Initialize(unsigned int initCount, unsigned int maxCount, const char* name = nullptr);
		cResult CleanUp();

		cSemaphore();
		~cSemaphore();


		// Implementation
		//=====================

	private:

		// Initialization / Clean Up
		//--------------------------

		cSemaphore(const cSemaphore& other) = delete;
		cSemaphore(cSemaphore&& other) = delete;
		cSemaphore& operator=(const cSemaphore& other) = delete;
		cSemaphore& operator=(cSemaphore&& other) = delete;


		// Data
		//=====================

	private:

		HANDLE m_handle;

	};


}// Namespace Concurrency
}// Namespace eae6320