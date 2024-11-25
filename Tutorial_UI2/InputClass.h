#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

#define KEYDOWN(keyState, keyCode) (keyState[keyCode] & 0x80) 

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>
#include <string>

class InputClass
{
public:
	InputClass();
	~InputClass();
	InputClass(const InputClass&);

	bool Initialize(HINSTANCE, HWND);
	bool Frame();
	void Shutdown();

	////////////////////Test///////////////////////
	void SetTextInputFocus(bool);
	bool GetTextInputFocus();
	const wchar_t* GetTextInputData();
	void AddTextInputData(const wchar_t text);
	void SubstractTextInputData();
	void ClearTextInputData();
	///////////////////////////////////////////////

	bool GetKeyPressed(const unsigned char keyCode);
	bool GetKeyPressedAndRelease(const unsigned char keyCode);
	bool GetKeyReleasedAndPress(const unsigned char keyCode);

	bool GetLeftMouseButtonPressed();
	bool GetLeftMouseButtonReleased();
	bool GetRightMouseButtonDown();
	bool GetRightMouseButtonUp();

	bool GetLeftMouseButtonPressAndReleased();
	bool GetLeftMouseButtonReleaseAndPressed();

	void GetMousePosition(long&, long&);
	void GetNormalizedMousePosition(float&, float&);
	void GetMouseDelta(float&, float&);

private:
	bool ReadKeyboard();
	bool ReadMouse();

private:
	LPDIRECTINPUT8 m_directInput;
	LPDIRECTINPUTDEVICE8 m_keyboardDevice, m_mouseDevice;
	DIMOUSESTATE2 m_mouseState;
	unsigned char m_currKeyState[256];
	unsigned char m_prevKeyState[256];

	bool m_prevMouseState[8];
	float m_sensitivity;

	std::wstring m_textInputData;
	bool m_textInputFocus;
};

#endif
