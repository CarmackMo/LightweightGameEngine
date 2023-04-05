#pragma once


inline InputManager::InputManager() :
	readBufferInitSize(128),
	readBufferSize(0),
	readBuffer(nullptr)
{
	Initialize();
}


inline InputManager::~InputManager()
{
	if (readBuffer)
		delete[] readBuffer;
}


inline void InputManager::Update()
{
	/* Reset the change state of all VKey as "Unchanged" */
	memset(VKeyChange, 0, sizeof(VKeyChange));
}


inline void InputManager::ResizeReadBuffer(size_t bytes)
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


inline bool InputManager::GetKeyDown(KeyCode keycode)
{
	return VKeyStates[keycode] == 1 && VKeyChange[keycode] == 1;
}


inline bool InputManager::GetKeyUp(KeyCode keycode)
{
	return VKeyStates[keycode] == 0 && VKeyChange[keycode] == 1;
}


inline bool InputManager::GetKey(KeyCode keycode)
{
	return VKeyStates[keycode] == 1;
}


inline void InputManager::AddOnKeyUpCallback(KeyCode keycode, void(*callback)(void))
{
	if (keyUpCallbacks.find(keycode) != keyUpCallbacks.end())
		keyUpCallbacks.at(keycode).push_back(callback);
	else
		keyUpCallbacks.emplace(keycode, vector<void(*)(void)>(1, callback));
}


inline void InputManager::AddOnKeyDownCallback(KeyCode keycode, void(*callback)(void))
{
	if (keyDownCallbacks.find(keycode) != keyDownCallbacks.end())
		keyDownCallbacks.at(keycode).push_back(callback);
	else
		keyDownCallbacks.emplace(keycode, vector<void(*)(void)>(1, callback));
}