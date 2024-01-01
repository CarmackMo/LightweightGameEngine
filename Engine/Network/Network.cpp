// Includes
//=========

#define WIN32_LEAN_AND_MEAN

#include <Engine/Logging/Logging.h>
#include <Engine/Network/Network.h>
#include <Engine/UserOutput/UserOutput.h>

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"



// Static Data
//============

namespace
{

	// Input Window
	//----------------------

	bool s_networkStatus = false;

	wchar_t s_className_input[] = L"InputWindow";

	HWND s_dialogWindow = NULL;
	HWND s_inputWindow = NULL;
	WPARAM s_onClickFlag = 0;

	// Network
	//----------------------

	eae6320::Network::eNetworkType s_networkType = eae6320::Network::eNetworkType::None;

	WSADATA s_wsaData;
	
	SOCKET s_connectSocket = INVALID_SOCKET;

}


// Helper Funcitons Forward Declaraction
//============

namespace eae6320
{
namespace Network
{
	eae6320::cResult Initialize_Host();

	eae6320::cResult Initialize_Client(std::string& i_hostIP);
}
}

namespace
{

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


eae6320::cResult eae6320::Network::CleanUp()
{
	if (s_connectSocket != INVALID_SOCKET)
		closesocket(s_connectSocket);

	WSACleanup();

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


eae6320::cResult eae6320::Network::SendData(const char* i_sendBuffer)
{
	if (GetNetworkTypeOfThisComputer() == eNetworkType::None)
	{
		UserOutput::ConsolePrint("Cannot send data without network connection \n");
		return Results::Failure;
	}
	else
	{
		int iResult = send(s_connectSocket, i_sendBuffer, (int)strlen(i_sendBuffer), 0);
		if (iResult == SOCKET_ERROR) 
		{
			Logging::OutputError("send failed with error: %d\n", WSAGetLastError());
			UserOutput::ConsolePrint(std::string("send failed with error: " + WSAGetLastError()).c_str());
			closesocket(s_connectSocket);
			WSACleanup();
			return Results::Failure;
		}
	}

	return Results::Success;
}


eae6320::Network::eNetworkType eae6320::Network::GetNetworkTypeOfThisComputer()
{
	return s_networkType;
}



// Helper Funcitons Implementation
//==================================

eae6320::cResult eae6320::Network::Initialize_Host()
{
	int iResult;

	struct addrinfo hints;
	struct addrinfo* result = NULL;
	
	SOCKET listenSocket = INVALID_SOCKET;

	// Initialize Winsock
	{
		iResult = WSAStartup(MAKEWORD(2, 2), &s_wsaData);
		if (iResult != 0) 
		{
			Logging::OutputError("WSAStartup failed with error: %d\n", iResult);
			UserOutput::ConsolePrint(std::string("WSAStartup failed with error: " + iResult).c_str());
			return Results::Failure;
		}
	}

	// Resolve the server address and port
	{
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
		if (iResult != 0) 
		{
			Logging::OutputError("getaddrinfo failed with error: %d\n", iResult);
			UserOutput::ConsolePrint(std::string("getaddrinfo failed with error: " + iResult).c_str());
			WSACleanup();
			return Results::Failure;
		}
	}

	// Create a SOCKET for the server to listen for client connections.
	{
		listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (listenSocket == INVALID_SOCKET)
		{
			Logging::OutputError("socket failed with error: %ld\n", WSAGetLastError());
			UserOutput::ConsolePrint(std::string("socket failed with error: " + WSAGetLastError()).c_str());
			freeaddrinfo(result);
			WSACleanup();
			return Results::Failure;
		}
	}

	// Setup the TCP listening socket
	{
		iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) 
		{
			Logging::OutputError("bind failed with error: %ld\n", WSAGetLastError());
			UserOutput::ConsolePrint(std::string("bind failed with error: " + WSAGetLastError()).c_str());
			freeaddrinfo(result);
			closesocket(listenSocket);
			WSACleanup();
			return Results::Failure;
		}

		freeaddrinfo(result);

		iResult = listen(listenSocket, SOMAXCONN);
		if (iResult == SOCKET_ERROR) 
		{
			Logging::OutputError("listen failed with error: %ld\n", WSAGetLastError());
			UserOutput::ConsolePrint(std::string("listen failed with error: " + WSAGetLastError()).c_str());
			closesocket(listenSocket);
			WSACleanup();
			return Results::Failure;
		}
	}

	// Accept a client socket
	{
		s_connectSocket = accept(listenSocket, NULL, NULL);
		if (s_connectSocket == INVALID_SOCKET)
		{
			Logging::OutputError("accept failed with error: %ld\n", WSAGetLastError());
			UserOutput::ConsolePrint(std::string("accept failed with error: " + WSAGetLastError()).c_str());
			closesocket(listenSocket);
			WSACleanup();
			return Results::Failure;
		}
	}

	// No longer need server socket
	closesocket(listenSocket);

	return Results::Success;
}


eae6320::cResult eae6320::Network::Initialize_Client(std::string& i_hostIP)
{
	int iResult;

	struct addrinfo hints;
	struct addrinfo* result = NULL;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &s_wsaData);
	if (iResult != 0) 
	{
		Logging::OutputError("WSAStartup failed with error: %d\n", iResult);
		UserOutput::ConsolePrint(std::string("WSAStartup failed with error: " + iResult).c_str());
		return Results::Failure;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(i_hostIP.c_str(), DEFAULT_PORT, &hints, &result);
	if (iResult != 0) 
	{
		Logging::OutputError("getaddrinfo failed with error: %d\n", iResult);
		UserOutput::ConsolePrint(std::string("getaddrinfo failed with error: " + iResult).c_str());
		WSACleanup();
		return Results::Failure;
	}

	// Attempt to connect to an address until one succeeds
	for (struct addrinfo* ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		s_connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (s_connectSocket == INVALID_SOCKET) 
		{
			Logging::OutputError("socket failed with error: %ld\n", WSAGetLastError());
			UserOutput::ConsolePrint(std::string("socket failed with error: " + WSAGetLastError()).c_str());
			WSACleanup();
			return Results::Failure;
		}

		// Connect to server.
		iResult = connect(s_connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) 
		{
			closesocket(s_connectSocket);
			s_connectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (s_connectSocket == INVALID_SOCKET) 
	{
		Logging::OutputError("Unable to connect to server!\n");
		UserOutput::ConsolePrint("Unable to connect to server!\n");
		WSACleanup();
		return Results::Failure;
	}

	return Results::Success;
}


namespace
{

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

				// Establish network connection
				if (ipAddr == "")
				{
					eae6320::Network::Initialize_Host();
					s_networkType = eae6320::Network::Server;
					s_networkStatus = true;
				}
				else if (eae6320::Network::Initialize_Client(ipAddr) == eae6320::Results::Success)
				{
					s_networkType = eae6320::Network::Client;
					s_networkStatus = true;
				}
				else
				{
					MessageBox(NULL, L"Connect to sever failed!", L"Connection Error", MB_OK);
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
