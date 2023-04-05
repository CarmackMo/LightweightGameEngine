#include "Input.h"

using namespace Engine::Input;


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
				Engine::Debugger::DEBUG_PRINT("Key 0x%02x Flags: 0x%04x\n", rawInput->data.keyboard.VKey, rawInput->data.keyboard.Flags);
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
