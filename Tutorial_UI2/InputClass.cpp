#include "InputClass.h"

InputClass::InputClass()
{
	m_directInput = 0;
	m_keyboardDevice = 0;
	m_mouseDevice = 0;
	m_sensitivity = 0.1f;
	m_textInputFocus = false;
}

InputClass::~InputClass()
{
}

InputClass::InputClass(const InputClass& other)
{
}

bool InputClass::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result;

	//DirectInput 생성
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, nullptr);
	if (FAILED(result))
	{
		return false;
	}

	//키보드용 Device 생성
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboardDevice, nullptr);
	if (FAILED(result))
	{
		return false;
	}

	//데이터 형식을 키보드 형식으로 설정
	result = m_keyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	//협력 수준 설정
	result = m_keyboardDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	//키보드 장치 획득
	result = m_keyboardDevice->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	//마우스용 Device 생성
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouseDevice, nullptr);
	if (FAILED(result))
	{
		return false;
	}

	//데이터 형식을 마우스 형식으로 설정
	result = m_mouseDevice->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(result))
	{
		return false;
	}

	//협력 수준 설정
	result = m_mouseDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	//마우스 장치 획득
	result = m_mouseDevice->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool InputClass::Frame()
{
	bool result;

	//마우스 State를 업데이트
	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	//키보드 State를 업데이트
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	return true;
}

void InputClass::Shutdown()
{
	//마우스 DirectDevice 해제
	if (m_mouseDevice)
	{
		m_mouseDevice->Unacquire();
		m_mouseDevice->Release();
		m_mouseDevice = NULL;
	}

	//키보드 DirectDevice 해제
	if (m_keyboardDevice)
	{
		m_keyboardDevice->Unacquire();
		m_keyboardDevice->Release();
		m_keyboardDevice = NULL;
	}

	//DirectInput 해제
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = NULL;
	}

	return;
}

bool InputClass::ReadKeyboard()
{
	HRESULT result;

	//키보드 장치의 상태를 가져옴
	result = m_keyboardDevice->GetDeviceState(sizeof(m_currKeyState), (LPVOID)&m_currKeyState);
	if (FAILED(result))
	{
		//DirectInputDevice에 문제가 생겼을 경우 장치를 다시 받아오는 부분
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboardDevice->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool InputClass::ReadMouse()
{
	HRESULT result;

	//마우스 장치의 상태를 가져옴
	result = m_mouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{

		//DirectInputDevice에 문제가 생겼을 경우 장치를 다시 받아오는 부분
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouseDevice->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

//마우스의 좌표를 얻는 함수
void InputClass::GetMousePosition(long& posX, long& posY)
{
	POINT pos;

	GetCursorPos(&pos);

	posX = pos.x;
	posY = pos.y;

	return;
}

//정규화된 마우스의 좌표를 얻는 함수
void InputClass::GetNormalizedMousePosition(float& posX, float& posY)
{
	return;
}

//마우스의 움직임을 얻는 함수
void InputClass::GetMouseDelta(float& posX, float& posY)
{
	posX = m_mouseState.lX * m_sensitivity;
	posY = m_mouseState.lY * m_sensitivity;

	return;
}

//키가 눌리면 true를 리턴하는 함수
bool InputClass::GetKeyPressed(const unsigned char keyCode)
{
	if (KEYDOWN(m_currKeyState, keyCode))
	{
		return true;
	}

	return false;
}

//키가 눌렸다가 땟을 때 true를 리턴하는 함수
bool InputClass::GetKeyPressedAndRelease(const unsigned char keyCode)
{
	if (!KEYDOWN(m_currKeyState, keyCode))
	{
		if (KEYDOWN(m_prevKeyState, keyCode))
		{
			m_prevKeyState[keyCode] = 0x00;
			return true;
		}
	}
	else
	{
		m_prevKeyState[keyCode] = 0x80;
	}

	return false;
}

//키가 땟다가 눌렸을 때 true를 리턴하는 함수
bool InputClass::GetKeyReleasedAndPress(const unsigned char keyCode)
{
	if (KEYDOWN(m_currKeyState, keyCode))
	{
		if (!KEYDOWN(m_prevKeyState, keyCode))
		{
			m_prevKeyState[keyCode] = 0x80;
			return true;
		}
	}
	else
	{
		m_prevKeyState[keyCode] = 0x00;
	}

	return false;
}

//마우스 왼쪽 버튼이 Press 상태인지 확인하는 함수
bool InputClass::GetLeftMouseButtonPressed()
{
	if (KEYDOWN(m_mouseState.rgbButtons, 0))
	{
		return true;
	}

	return false;
}

//마우스 왼쪽 버튼이 Release 상태인지 확인하는 함수
bool InputClass::GetLeftMouseButtonReleased()
{
	if (!KEYDOWN(m_mouseState.rgbButtons, 0))
	{
		return true;
	}

	return false;
}

//Press 상태에서 Release 상태로 전환 됐는지 확인하는 함수
bool InputClass::GetLeftMouseButtonPressAndReleased()
{
	if (!KEYDOWN(m_mouseState.rgbButtons, 0))
	{
		if (m_prevMouseState[0])
		{
			m_prevMouseState[0] = false;
			return true;
		}
	}
	else
	{
		m_prevMouseState[0] = true;
	}

	return false;
}

//Release 상태에서 Press 상태로 전환 됐는지 확인하는 함수
bool InputClass::GetLeftMouseButtonReleaseAndPressed()
{
	if (KEYDOWN(m_mouseState.rgbButtons, 0))
	{
		if (!m_prevMouseState[0])
		{
			m_prevMouseState[0] = true;
			return true;
		}
	}
	else
	{
		m_prevMouseState[0] = false;
	}

	return false;
}

//마우스 오른쪽 버튼이 눌렸는지 체크하는 함수
bool InputClass::GetRightMouseButtonDown()
{
	if (m_mouseState.rgbButtons[1] & 0x80)
	{
		return true;
	}

	return false;
}


//마우스 오른쪽 버튼이 눌렸다 떗는지 체크하는 함수
bool InputClass::GetRightMouseButtonUp()
{
	if (!KEYDOWN(m_mouseState.rgbButtons, 1))
	{
		if (m_prevMouseState[1])
		{
			m_prevMouseState[1] = false;
			return true;
		}
	}
	else
	{
		m_prevMouseState[1] = true;
	}

	return false;
}

///////////////////////////////Test///////////////////////////
//텍스트 입력 포커스 설정
void InputClass::SetTextInputFocus(bool state)
{
	m_textInputFocus = state;

	//포커스 상태가 바뀌면 여태 입력했던 데이터 제거
	ClearTextInputData();
}

//텍스트 입력 포커스 확인
bool InputClass::GetTextInputFocus()
{
	if (this == nullptr)//static 콜백 함수에서 동적 할당되지 않는 상태에서 호출하는 문제를 방지하기 위해 넣음
	{
		return false;
	}

	return m_textInputFocus;
}

//배열에 글자 저장
void InputClass::AddTextInputData(const wchar_t text)
{
	m_textInputData += text;
}

//가장 마지막 글자 제거
void InputClass::SubstractTextInputData()
{
	m_textInputData.pop_back();
}

//텍스트 배열 반환
const wchar_t* InputClass::GetTextInputData()
{
	return m_textInputData.c_str();
}

//텍스트 배열 초기화
void InputClass::ClearTextInputData()
{
	m_textInputData.clear();
}