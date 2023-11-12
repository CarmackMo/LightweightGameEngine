/*
	This is the base class for an application using this engine

	It exposes virtual platform-independent functions for the actual application
	and takes care of platform-specific application tasks.
*/

#ifndef EAE6320_APPLICATION_IAPPLICATION_H
#define EAE6320_APPLICATION_IAPPLICATION_H

// Includes
//=========

#include <cstdint>
#include <Engine/Concurrency/cThread.h>
#include <Engine/Results/Results.h>

#if defined( EAE6320_PLATFORM_WINDOWS )
	#include <Engine/Windows/Includes.h>
#endif

// Forward Declarations
//=====================

namespace eae6320
{
	namespace Graphics
	{
		struct sInitializationParameters;
	}
	namespace UserOutput
	{
		struct sInitializationParameters;
	}
}

// Class Declaration
//==================

namespace eae6320
{
namespace Application
{
	// The only thing that a specific application project's main() entry point should do
	// is to call the following function with the derived application class
	// as the template argument:
	template<class tApplication>
	int Run(
#if defined( EAE6320_PLATFORM_WINDOWS )
			const HINSTANCE i_thisInstanceOfTheApplication, const char* const i_commandLineArguments, const int i_initialWindowDisplayState
#endif
	)
	{
		// Create a platform-specific struct
		// that can be passed to functions in a platform-independent way
		tApplication::sEntryPointParameters entryPointParameters =
		{
#if defined( EAE6320_PLATFORM_WINDOWS )
			i_thisInstanceOfTheApplication, i_commandLineArguments, i_initialWindowDisplayState
#endif
		};
		// Create an instance of the applciation
		tApplication newApplicationInstance;
		// Run it
		return newApplicationInstance.ParseEntryPointParametersAndRun( entryPointParameters );
	}



	class iApplication
	{
		// Interface
		//==========

	public:

		// Different platforms have different parameters that get passed to a program's entry point
		struct sEntryPointParameters
		{
#if defined( EAE6320_PLATFORM_WINDOWS )
			// The specific instance of the application
			// (if you have two instances of your game running simultaneously
			// this handle can differentiate between them)
			const HINSTANCE applicationInstance = NULL;
			// The arguments in the command to start the program
			const char* const commandLineArguments = nullptr;
			// The requested initial state of the window
			// (e.g. minimized, maximized, etc.)
			const int initialWindowDisplayState = SW_SHOWNORMAL;
#endif
		};

		// Access
		//-------

		cResult GetCurrentResolution( uint16_t& o_width, uint16_t& o_height ) const;
		double GetElapsedSecondCount_systemTime() const;
		double GetElapsedSecondCount_simulation() const;
		void SetSimulationRate( const float i_simulationRate );

		// Run
		//------

		// This is called from the templated Run<> function above
		// with the parameters directly from the main() entry point
		// and returns the exit code that the entry point will return
		int ParseEntryPointParametersAndRun( const sEntryPointParameters& i_entryPointParameters );

		// Exits the application
		// (a derived class can call this explicitly when it determines the application should exit)
		cResult Exit( const int i_exitCode );

		// Initialize / Clean Up
		//----------------------

		iApplication();
		virtual ~iApplication() = 0;

		// Inheritable Implementation
		//===========================

	private:

		// Configuration
		//--------------

		// Your application can override the following configuration functions if desired

		virtual void GetDefaultInitialResolution( uint16_t& o_width, uint16_t& o_height ) const
		{
			o_width = 1300;
			o_height = 900;
		}

		// The application's simulation is always updated in fixed increments of time
		// to keep the simulation stable and predictable.
		// This function determines how frequently the simulation is updated
		// (the simulation will be updated every n seconds, where n is the value returned by this function).
		// The default value is a relatively large amount of time;
		// you may want to experiement with smaller values in your application
		// and observe the change in responsiveness or simulation accuracy.
		virtual float GetSimulationUpdatePeriod_inSeconds() const { return 1.0f / 15.0f; }

		// Run
		//----

		// There are two different timelines that effect your application:
		//	* System Time
		//		* This is "real" time, as perceived by the user.
		//			It keeps progressing while the application is running
		//			(e.g. system time continues to elapse even when a game is paused)
		//			but stops if the application itself isn't running
		//			(e.g. if you are debugging and the application hits a breakpoint)
		//	* Simulation Time
		//		* This is the time within the application's simulation,
		//			and it is updated with fixed time steps
		//			and doesn't necessarily match system time
		//			(e.g. it stops completely when the game is paused,
		//			and your application could intentionally run it and a slower or faster rate)

		// There are two different ways that the state of your application can be changed:
		//	* Input
		//		* This is an external state change. It could be a result of user input,
		//			but it could also be the result of an event or a game entity's AI.
		//	* Time
		//		* Some state will change instantaneously, but many things in your application
		//			(particularly things being simulated)
		//			will be changing gradually, and so whenever time passes they must be updated.

		// Your application can implement the following two functions
		// to control the update of non-simulation state
		// (in a game this would be e.g. the UI).
		// They will both be called every rendered frame.
		virtual void UpdateBasedOnInput() {}
		virtual void UpdateBasedOnTime( const float i_elapsedSecondCount_sinceLastUpdate ) {}

		// Your application can implement the following two functions
		// to control the update of the simulation.
		// They will both be called every time the simulation is updated
		// (i.e. whenever GetSimulationUpdatePeriod_inSeconds() of simulation time has elapsed)
		virtual void UpdateSimulationBasedOnInput() {}
		virtual void UpdateSimulationBasedOnTime( const float i_elapsedSecondCount_sinceLastUpdate ) {}

		// Your application should override the following function
		// to instuct the Graphics system what to render for the next frame
		virtual void SubmitDataToBeRendered( const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate ) {}

		// Initialize / Clean Up
		//----------------------

		// These functions are for initializing and cleaning up application-specific systems and data
		// (i.e. initializing the engine and application base class happens automatically;
		// a derived class only needs to worry about itself)
		virtual cResult Initialize() = 0;
		virtual cResult CleanUp() = 0;

		// Data
		//=====

	private:

		// The time when the application started
		// (This is set during initialization and then never changed)
		uint64_t m_tickCount_systemTime_whenApplicationStarted = 0;
		// The current time
		// (this is updated at the beginning of each iteration of the application loop).
		// The total time that the application has been running can be calculated by:
		//		m_tickCount_systemTime_current - m_tickCount_systemTime_whenApplicationStarted
		uint64_t m_tickCount_systemTime_current = 0;
		// The total amount of "simulation time" that has elapsed:
		//	* As opposed to "system time", which passes independently of your application,
		//		"simulation time" is a measure of how much time is passing within your application's simulation.
		//		This can be different from system time, for example:
		//			* Your application is paused
		//			* Your game enters a slow motion mode
		//	* This "total elapsed" reflects how much simulation time has elapsed since a certain arbitrary point in time.
		//		It can be how much time has been simulated during the entire application,
		//		but there is no technical reason that it couldn't be reset (e.g. when starting a new level).
		uint64_t m_tickCount_simulationTime_totalElapsed = 0;
		// The thread that runs the main application loop
		// (The original process thread (or "main thread") services operating system requests and the render loop,
		// because many operating systems require those to use the same thread that they were created/initialized with)
		Concurrency::cThread m_applicationLoopThread;
		// The rate that simulation time elapses relative to system time.
		// At its default value of 1 the simulation runs in real time
		// (this is usually what you want).
		// At larger values the simulation runs faster than system time
		// and at smaller values it runs more slowly.
		// (For example, when this value is 0 the simulation will stop,
		// when this value is 0.5 the simulation will run at half the speed of system time,
		// and when this value is 2 the simulation will run twice as fast as system time.)
		float m_simulationRate = 1.0f;
		// The application loop thread checks this variable every iteration
		// so that it knows if the main thread requires it to exit
		bool m_shouldApplicationLoopExit = false;

		// Implementation
		//===============

	private:

		// Run
		//----

		// This is called after the application has been initialized,
		// and it returns once the application is finished and ready to exit.
		// It runs on the original process thread
		// and interacts with the operating system to decide when 
		// the graphics system can render a frame.
		// This function will be called with the same rate of operating system's tick.
		cResult RenderFramesWhileWaitingForApplicationToExit( int& o_exitCode );

		// The main application loop.
		// It is called from its own thread (m_applicationLoopThread)
		// distinct from the main process thread (that is used to render).
		void UpdateUntilExit();
		static void EntryPoint_applicationLoopThread( void* const io_application );

		cResult Exit_platformSpecific( const int i_exitCode );

		// Initialize / Clean Up
		//----------------------

		cResult Initialize_all( const sEntryPointParameters& i_entryPointParameters );	// This initializes everything
		cResult Initialize_base( const sEntryPointParameters& i_entryPointParameters );	// This initializes just this base class
		cResult Initialize_engine();	// This initializes all of the engine systems

		cResult PopulateGraphicsInitializationParameters( Graphics::sInitializationParameters& o_initializationParameters );
		cResult PopulateUserOutputInitializationParameters( UserOutput::sInitializationParameters& o_initializationParameters );

		cResult CleanUp_all();	// This cleans up everything
		cResult CleanUp_base();	// This cleans up just this base class
		cResult CleanUp_engine();	// This cleans up all of the engine systems

		// #include the platform-specific class declarations
#if defined( EAE6320_PLATFORM_WINDOWS )
		#include "Windows/iApplication.win.h"
#endif
	};
}
}

#endif	// EAE6320_APPLICATION_IAPPLICATION_H
