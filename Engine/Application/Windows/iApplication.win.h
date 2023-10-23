/*
	This file contains platform-specific class declarations for the application
*/

#ifndef EAE6320_APPLICATION_IAPPLICATION_WIN_H
#define EAE6320_APPLICATION_IAPPLICATION_WIN_H

	// Data
	//=====

private:

	HWND m_mainWindow = NULL;
	ATOM m_mainWindowClass = NULL;
	HINSTANCE m_thisInstanceOfTheApplication = NULL;

	int m_exitCode = 0;

	uint16_t m_resolutionWidth = 0;
	uint16_t m_resolutionHeight = 0;

	// Inherited Implementation
	//=========================

protected:

	// Access
	//-------

	static iApplication* GetApplicationFromWindow( const HWND i_window );

	// Inheritable Implementation
	//===========================

private:

	// Configuration
	//--------------

	// Your application can override the following configuration functions

	virtual const char* GetMainWindowName() const = 0;
	virtual const char* GetMainWindowClassName() const = 0;
	virtual const WORD* GetLargeIconId() const { return nullptr; }
	virtual const WORD* GetSmallIconId() const { return nullptr; }

	// Implementation
	//===============

private:

	// Run
	//----

	static LRESULT CALLBACK OnMessageReceivedFromWindows( HWND i_window, UINT i_message, WPARAM i_wParam, LPARAM i_lParam );

#endif	// EAE6320_APPLICATION_IAPPLICATION_WIN_H
