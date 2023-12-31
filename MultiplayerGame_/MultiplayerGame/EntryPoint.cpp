/*
	WinMain() is the entry point of a Windows program
	(instead of the familiar main() in a console program)
*/

// Includes
//=========

#include <MultiplayerGame_/MultiplayerGame/cMultiplayerGame.h>

// Entry Point
//============

int WINAPI WinMain( HINSTANCE i_thisInstanceOfTheApplication, HINSTANCE, char* i_commandLineArguments, int i_initialWindowDisplayState )
{
	return eae6320::Application::Run<MultiplayerGame::cMultiplayerGame>( i_thisInstanceOfTheApplication, i_commandLineArguments, i_initialWindowDisplayState );
}
