// Includes
//=========

#include <Engine/Network/Network.h>

#include <string>


// Static Data
//============

namespace
{
	bool s_networkStatus = false;

	wchar_t s_className_input[] = L"InputWindow";

	HWND s_dialogWindow = NULL;
	HWND s_inputWindow = NULL;
	WPARAM s_onClickFlag = 0;
}


// Helper Funcitons Forward Declaraction
//============

namespace
{

	eae6320::cResult Initialize_Host();

	eae6320::cResult Initialize_Client();

	eae6320::cResult ConnectToHost();



	LRESULT CALLBACK WIndowProcedure(HWND, UINT, WPARAM, LPARAM);

}



// Interface Implementation
//============

eae6320::cResult eae6320::Network::Initialize(HINSTANCE i_hInstance)
{
	// Register a new class for input window
	{
		WNDCLASSW wc = { 0 };

		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hInstance = i_hInstance;
		wc.lpszClassName = s_className_input;
		wc.lpfnWndProc = WIndowProcedure;

		if (!RegisterClassW(&wc))
			return Results::Failure;
	}

	return Results::Success;
}


eae6320::cResult eae6320::Network::Connect()
{
	s_dialogWindow = CreateWindowW(
		s_className_input,
		L"My Window",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		100, 100,
		500, 500,
		NULL, NULL, NULL, NULL);

	MSG msg = { 0 };

	while (GetMessage(&msg, NULL, NULL, NULL) && s_networkStatus == false)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}


	DestroyWindow(s_dialogWindow);
	return Results::Success;
}



// Helper Funcitons Implementation
//==================================

namespace
{

	eae6320::cResult ConnectToHost()
	{
		return eae6320::Results::Success;
	}


	LRESULT CALLBACK WIndowProcedure(HWND i_hWnd, UINT i_msg, WPARAM i_wp, LPARAM i_lp)
	{
		if (i_msg == WM_COMMAND)
		{
			if (i_wp == s_onClickFlag)
			{
				wchar_t text[100];
				GetWindowTextW(s_inputWindow, text, 100);

				std::string ipAddr;
				for (wchar_t ch : text)
				{
					if (ch == '\0') { break; }

					ipAddr.push_back(static_cast<char>(ch));
				}


				if (ipAddr == "")
				{
					//Initialize_Host();
					s_networkStatus = true;
				}
				else
				{
					//Initialize_Client();
					s_networkStatus = true;
				}

			}
		}
		else if (i_msg == WM_CREATE)
		{
			// Static text
			CreateWindowW(
				L"Static", L"Enter host's IP address if your are client, leave blank if you are server:",
				WS_VISIBLE | WS_CHILD,
				100, 100,
				300, 50,
				i_hWnd, NULL, NULL, NULL);

			// User input box
			s_inputWindow = CreateWindowW(
				L"Edit", L"",
				WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL,
				100, 150,
				300, 50,
				i_hWnd, NULL, NULL, NULL);

			// Button
			CreateWindowW(
				L"Button", L"Connect",
				WS_VISIBLE | WS_CHILD,
				100, 210,
				100, 50,
				i_hWnd, (HMENU)s_onClickFlag, NULL, NULL);
		}
		else if (i_msg == WM_DESTROY) { }
		else if (i_msg == WM_CLOSE) { }

		return DefWindowProcW(i_hWnd, i_msg, i_wp, i_lp);
	}
}
