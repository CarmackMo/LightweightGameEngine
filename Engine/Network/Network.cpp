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
	WPARAM s_inputFlag = 0;
}


// Helper Funcitons Forward Declaraction
//============

namespace
{

	eae6320::cResult Initialize_Host();

	eae6320::cResult Initialize_Client();

	eae6320::cResult ConnectToHost();



	void CreateInputWindow();

	LRESULT CALLBACK WIndowProcedure(HWND, UINT, WPARAM, LPARAM);

}



// Interface Implementation
//============

eae6320::cResult eae6320::Network::Initialize(HINSTANCE i_hInstance)
{

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
	MSG msg = { 0 };

	CreateInputWindow();

	while (GetMessage(&msg, NULL, NULL, NULL) && s_networkStatus == false)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

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


	void CreateInputWindow()
	{
		s_dialogWindow = CreateWindowW(
			s_className_input,
			L"My Window",
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			100, 100,
			500, 500,
			NULL, NULL, NULL, NULL);
	}


	LRESULT CALLBACK WIndowProcedure(HWND i_hWnd, UINT i_msg, WPARAM i_wp, LPARAM i_lp)
	{
		if (i_msg == WM_COMMAND)
		{
			if (i_wp == s_inputFlag)
			{
				wchar_t text[100];
				GetWindowTextW(s_inputWindow, text, 100);

				//std::wstring temp(text);
				//std::string ipAddr = std::string(temp.begin(), temp.end());

				//if (ipAddr == "")
				//{
				//	s_networkStatus = true;
				//}
				//else
				//{
				//	ConnectToHost();
				//}


				s_networkStatus = true;
				DestroyWindow(i_hWnd);
			}
		}
		else if (i_msg == WM_CREATE)
		{
			// Static text
			CreateWindowW(
				L"Static", L"Enter host's IP address if your are client, leave blank if you are server:",
				WS_VISIBLE | WS_CHILD,
				200, 100,
				100, 50,
				i_hWnd, NULL, NULL, NULL);

			// User input box
			s_inputWindow = CreateWindowW(
				L"Edit", L"",
				WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL,
				200, 150,
				100, 50,
				i_hWnd, NULL, NULL, NULL);

			// Button
			CreateWindowW(
				L"Button", L"Connect",
				WS_VISIBLE | WS_CHILD,
				200, 210,
				100, 50,
				i_hWnd, (HMENU)s_inputFlag, NULL, NULL);
		}
		else if (i_msg == WM_DESTROY)
		{
		}
		else if (i_msg == WM_CLOSE)
		{
		}

		return DefWindowProcW(i_hWnd, i_msg, i_wp, i_lp);
	}
}
