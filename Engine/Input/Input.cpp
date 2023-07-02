#include <cassert>
#include "Input.h"
#include "Debugger.h"

namespace Engine
{
namespace Input
{

using std::vector;
using std::unordered_map;


InputManager::InputManager() :
	readBufferInitSize(128),
	readBufferSize(0),
	readBuffer(nullptr)
{
	Initialize();
}


InputManager::~InputManager()
{
	if (readBuffer)
		delete[] readBuffer;
}


void InputManager::Initialize()
{
	assert(readBuffer == nullptr);
	ResizeReadBuffer(readBufferInitSize);

	RAWINPUTDEVICE keyboard;
	keyboard.usUsagePage = 0x01;		// Generic desktop page
	keyboard.usUsage = 0x06;			// Keyboard
	keyboard.dwFlags = RIDEV_NOLEGACY;	// Turn off keyboard input to the Windows message handlers
	keyboard.hwndTarget = NULL;

	BOOL success = RegisterRawInputDevices(&keyboard, 1, sizeof(keyboard));
	assert(success == TRUE);

	memset(VKeyStates, 0, sizeof(VKeyStates));
	memset(VKeyChange, 0, sizeof(VKeyChange));
}


void InputManager::Service(HRAWINPUT input)
{
	/* Check the size of the input data, in case buffer is not big enough to hold it */
	UINT bytesData = 0;
	UINT ret = GetRawInputData(input, RID_INPUT, NULL, &bytesData, sizeof(RAWINPUTHEADER));
	assert(ret == 0);

	if (bytesData)
	{
		ResizeReadBuffer(bytesData);

		ret = GetRawInputData(input, RID_INPUT, readBuffer, &bytesData, sizeof(RAWINPUTHEADER));
		assert(ret == bytesData);

		RAWINPUT* rawInput = reinterpret_cast<RAWINPUT*>(readBuffer);

		/* If the input data is from a keyboard input */
		if (rawInput->header.dwType == RIM_TYPEKEYBOARD)
		{
			USHORT VKey = rawInput->data.keyboard.VKey;
			USHORT Flags = rawInput->data.keyboard.Flags;
			
			/* If the key is up */
			if (Flags == RI_KEY_BREAK)
			{
				bool change = VKeyStates[VKey] != 0;
				VKeyStates[VKey] = 0;

				if (change)
				{
					VKeyChange[VKey] = 1;

					/* Invoke registed callbacks */
					if (keyUpCallbacks.find((KeyCode)VKey) != keyUpCallbacks.end())
					{
						for (void(*callback)(void) : keyUpCallbacks.at((KeyCode)VKey))
						{
							callback();
						}
					}
					//Engine::Debugger::DEBUG_PRINT("Key 0x%02x changed state to UP\n", rawInput->data.keyboard.VKey);
				}
			}
			/* If the key is down */
			else if (Flags == RI_KEY_MAKE)
			{
				bool change = VKeyStates[VKey] != 1;
				VKeyStates[VKey] = 1;

				if (change)
				{
					VKeyChange[VKey] = 1;

					/* Invoke registed callbacks */
					if (keyDownCallbacks.find((KeyCode)VKey) != keyDownCallbacks.end())
					{
						for (void(*callback)(void) : keyDownCallbacks.at((KeyCode)VKey))
						{
							callback();
						}
					}
					//Engine::Debugger::DEBUG_PRINT("Key 0x%02x changed state to DOWN\n", rawInput->data.keyboard.VKey);
				}
			}
			/* Special case */
			else 
			{
				DEBUG_PRINT("Key 0x%02x Flags: 0x%04x\n", rawInput->data.keyboard.VKey, rawInput->data.keyboard.Flags);
			}
		}
	}
}


void InputManager::RemoveOnKeyUpCallback(KeyCode keycode, void(*callback)(void))
{
	vector<void(*)(void)> callbacks = keyUpCallbacks.at(keycode);
	for (auto iter = callbacks.begin(); iter < callbacks.end(); iter++)
	{
		if (*iter == callback)
		{
			callbacks.erase(iter);
			break;
		}
	}
}


void InputManager::RemoveOnKeyDownCallback(KeyCode keycode, void(*callback)(void))
{
	vector<void(*)(void)> callbacks = keyDownCallbacks.at(keycode);
	for (auto iter = callbacks.begin(); iter < callbacks.end(); iter++)
	{
		if (*iter == callback)
		{
			callbacks.erase(iter);
			break;
		}
	}
}


void InputManager::ResizeReadBuffer(size_t bytes)
{
	assert(bytes);

	if (bytes > readBufferSize)
	{
		if (readBuffer != nullptr)
			delete[] readBuffer;

		readBuffer = new BYTE[bytes];
		assert(readBuffer);

		readBufferSize = bytes;
	}
}


void InputManager::AddOnKeyUpCallback(KeyCode keycode, void(*callback)(void))
{
	if (keyUpCallbacks.find(keycode) != keyUpCallbacks.end())
		keyUpCallbacks.at(keycode).push_back(callback);
	else
		keyUpCallbacks.emplace(keycode, vector<void(*)(void)>(1, callback));
}


void InputManager::AddOnKeyDownCallback(KeyCode keycode, void(*callback)(void))
{
	if (keyDownCallbacks.find(keycode) != keyDownCallbacks.end())
		keyDownCallbacks.at(keycode).push_back(callback);
	else
		keyDownCallbacks.emplace(keycode, vector<void(*)(void)>(1, callback));
}

}//Namespace Input
}//Namespace Engine
