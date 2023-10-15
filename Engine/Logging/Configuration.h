/*
	This file provides configurable settings
	that can be used to control logging behavior
*/

#ifndef EAE6320_LOGGING_CONFIGURATION_H
#define EAE6320_LOGGING_CONFIGURATION_H

// Unless you change the path here the log will be generated
// in the same directory as the game's executable
// (which can be nice because it is easy for a user to find)
#define EAE6320_LOGGING_PATH "eae6320.log"

// Flushing the logging buffer to disk is expensive,
// but it can be done after every message is output during development
// if an application is crashing so that no messages are lost
#ifdef _DEBUG
	#define EAE6320_LOGGING_FLUSHBUFFERAFTEREVERYMESSAGE
#endif

#endif	// EAE6320_LOGGING_CONFIGURATION_H
