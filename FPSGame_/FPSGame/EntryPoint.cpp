/*
	WinMain() is the entry point of a Windows program
	(instead of the familiar main() in a console program)
*/

// Includes
//=========

#include "cFPSGame.h"

// Entry Point
//============

int WINAPI WinMain( HINSTANCE i_thisInstanceOfTheApplication, HINSTANCE, char* i_commandLineArguments, int i_initialWindowDisplayState )
{
	return eae6320::Application::Run<eae6320::cFPSGame>( i_thisInstanceOfTheApplication, i_commandLineArguments, i_initialWindowDisplayState );
}
