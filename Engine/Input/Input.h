#pragma once
#include "Dependency.h"
#include "DataType.h"

namespace Engine
{
namespace Input
{

enum KeyCode
{
	BackSpace = 0x08,	// BACKSPACE key
	Tab = 0x09,			// TAB key
	Clear = 0x0C,		// CLEAR key
	Enter = 0x0D,		// ENTER key

	Shift = 0x10,		// SHIFT key
	LShift = 0xA0,		// LEFT SHIFT key
	RShift = 0xA1,		// RIGHT SHIFT key
	
	Ctrl = 0x11,		// CTRL key
	LCtrl = 0xA2,		// LEFT CTRL key
	RCtrl = 0xA3,		// RIGHT CTRL key
	
	Alt = 0x12,			// ALT key
	LAlt = 0xA4,		// LEFT ALT key
	RAlt = 0xA5,		// RIGHT ALT key

	LWin = 0x5B,		// Left Window key
	RWin = 0x5C,		// Right Window key

	Pause = 0x13,		// PAUSE key
	CapsLock = 0x14,	// CAPS LOCK key
	Escape = 0x1B,		// ESC key
	Space = 0x20,		// SPACE bar
	PageUp = 0x21,		// PAGE UP key
	PageDown = 0x22,	// PAGE DOWN key
	End = 0x23,			// END key
	Home = 0x24,		// HOME key
	Insert = 0x2D,		// INSERT key
	Delete = 0x2E,		// DELETE key

	Left = 0x25,		// LEFT ARROW key
	UP = 0x26,			// UP ARROW key
	RIGHT = 0x27,		// RIGHT ARROW key
	DOWN = 0x28,		// DOWN ARROW key

	Num0 = 0x30,
	Num1 = 0x31,
	Num2 = 0x32,
	Num3 = 0x33,
	Num4 = 0x34,
	Num5 = 0x35,
	Num6 = 0x36,
	Num7 = 0x37,
	Num8 = 0x38,
	Num9 = 0x39,
	NumPad0 = 0x60,		// Numeric keypad 0 key
	NumPad1 = 0x61,
	NumPad2 = 0x62,
	NumPad3 = 0x63,
	NumPad4 = 0x64,
	NumPad5 = 0x65,
	NumPad6 = 0x66,
	NumPad7 = 0x67,
	NumPad8 = 0x68,
	NumPad9 = 0x69,

	A = 0x41,
	B = 0x42,
	C = 0x43,
	D = 0x44,
	E = 0x45,
	F = 0x46,
	G = 0x47,
	H = 0x48,
	I = 0x49,
	J = 0x4A,
	K = 0x4B,
	L = 0x4C,
	M = 0x4D,
	N = 0x4E,
	O = 0x4F,
	P = 0x50,
	Q = 0x51,
	R = 0x52,
	S = 0x53,
	T = 0x54,
	U = 0x55,
	V = 0x56,
	W = 0x57,
	X = 0x58,
	Y = 0x59,
	Z = 0x5A,

	F1 = 0x70,
	F2 = 0x71,
	F3 = 0x72,
	F4 = 0x73,
	F5 = 0x74,
	F6 = 0x75,
	F7 = 0x76,
	F8 = 0x77,
	F9 = 0x78,
	F10 = 0x79,
	F11 = 0x7A,
	F12 = 0x7B,
	F13 = 0x7C,
	F14 = 0x7D,
};


class InputManager : public Singleton<InputManager>
{
private:
	const size_t readBufferInitSize = 128;
	size_t readBufferSize = 0;
	LPBYTE readBuffer = nullptr;

	uint8_t VKeyStates[256]; /* 0 represents key is released, 1 represents key is pressed */
	uint8_t VKeyChange[256]; /* 0 represents key does not have a state change, 1 represents key has a state change */

	unordered_map<KeyCode, vector<void(*)(void)>> keyUpCallbacks;		/* Store function pointers */
	unordered_map<KeyCode, vector<void(*)(void)>> keyDownCallbacks;

	inline InputManager();
	inline ~InputManager();

public:
	friend class Singleton<InputManager>;

	inline void Update();
	inline void ResizeReadBuffer(size_t bytes);
	void Initialize();
	void Service(HRAWINPUT input);

	/**	
	 *	@brief Return TRUE on the frame when the given key is pressed down 
	 */
	inline bool GetKeyDown(KeyCode keycode);
	/**
	 *	@brief Return TRUE on the frame when the given key is released up
	 */
	inline bool GetKeyUp(KeyCode keycode);
	/**
	 *	@brief Return TRUE when the given key is pressed
	 */
	inline bool GetKey(KeyCode keycode);

	/**
	 *	@brief Register function to call list. Function is called on the 
	 *		   frame when the given key is released up.
	 *  @param callback: pointer to the function.
	 */
	inline void AddOnKeyUpCallback(KeyCode keycode, void(*callback)(void));
	/**
	 *	@brief Register function to call list. Function is called on the
	 *		   frame when the given key is pressed down.
	 *  @param callback: pointer to the function.
	 */
	inline void AddOnKeyDownCallback(KeyCode keycode, void(*callback)(void));

	void RemoveOnKeyUpCallback(KeyCode keycode, void(*callback)(void));
	void RemoveOnKeyDownCallback(KeyCode keycode, void(*callback)(void));
};

#include "Input.inl"

}
}