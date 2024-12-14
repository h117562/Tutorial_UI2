#include "InputClass.h"

InputClass::InputClass()
{
	m_directInput = 0;
	m_keyboardDevice = 0;
	m_mouseDevice = 0;
	m_sensitivity = 0.1f;
	m_textInputEnabled = false;
}

InputClass::~InputClass()
{

	//���콺 DirectDevice ����
	if (m_mouseDevice)
	{
		m_mouseDevice->Unacquire();
		m_mouseDevice->Release();
		m_mouseDevice = NULL;
	}

	//Ű���� DirectDevice ����
	if (m_keyboardDevice)
	{
		m_keyboardDevice->Unacquire();
		m_keyboardDevice->Release();
		m_keyboardDevice = NULL;
	}

	//DirectInput ����
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = NULL;
	}
}

bool InputClass::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result;

	m_hwnd = hwnd;

	//DirectInput ����
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, nullptr);
	if (FAILED(result))
	{
		return false;
	}

	//Ű����� Device ����
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboardDevice, nullptr);
	if (FAILED(result))
	{
		return false;
	}

	//������ ������ Ű���� �������� ����
	result = m_keyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	//���� ���� ����
	result = m_keyboardDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	//Ű���� ��ġ ȹ��
	result = m_keyboardDevice->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	//���콺�� Device ����
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouseDevice, nullptr);
	if (FAILED(result))
	{
		return false;
	}

	//������ ������ ���콺 �������� ����
	result = m_mouseDevice->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(result))
	{
		return false;
	}

	//���� ���� ����
	result = m_mouseDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	//���콺 ��ġ ȹ��
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

	//���콺 State�� ������Ʈ
	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	//Ű���� State�� ������Ʈ
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	return true;
}

bool InputClass::ReadKeyboard()
{
	HRESULT result;

	//���� Ű ���¸� ���� ���·� �����
	memcpy(m_prevKeyState, m_currKeyState, 256);

	//���� Ű���� ��ġ�� ���¸� ������
	result = m_keyboardDevice->GetDeviceState(sizeof(m_currKeyState), (LPVOID)&m_currKeyState);
	if (FAILED(result))
	{
		//DirectInputDevice�� ������ ������ ��� ��ġ�� �ٽ� �޾ƿ��� �κ�
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

	//���� ���콺 ��ư ���¸� ���� ���·� �����
	memcpy(m_prevMouseState, m_mouseState.rgbButtons, 8);

	//���콺 ��ġ�� ���¸� ������
	result = m_mouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{

		//DirectInputDevice�� ������ ������ ��� ��ġ�� �ٽ� �޾ƿ��� �κ�
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

//��üȭ�� ���� ���콺�� ��ǥ�� ��� �Լ�
void InputClass::GetMousePosition(long& posX, long& posY)
{
	POINT pos;

	GetCursorPos(&pos);

	posX = pos.x;
	posY = pos.y;

	return;
}

//������ �������� ����ȭ�� ���콺�� ��ǥ�� ��� �Լ�
void InputClass::GetNormalizedMousePosition(float& posX, float& posY)
{
	POINT pos;
	RECT rect;

	//��üȭ�� ���� ��ǥ
	GetCursorPos(&pos);

	ScreenToClient(m_hwnd, &pos);
	GetClientRect(m_hwnd, &rect);

	//���콺 ��ġ�� ����ȭ
	posX = ((2.0f * static_cast<float>(pos.x)) / static_cast<float>(rect.right - rect.left)) - 1.0f;
	posY = ((-2.0f * static_cast<float>(pos.y)) / static_cast<float>(rect.bottom - rect.top)) + 1.0f;

	return;
}

//���콺�� �������� ��� �Լ�
void InputClass::GetMouseDelta(float& posX, float& posY)
{
	posX = m_mouseState.lX * m_sensitivity;
	posY = m_mouseState.lY * m_sensitivity;

	return;
}

//Ű�� ������ true�� �����ϴ� �Լ�
bool InputClass::GetKeyPressed(const unsigned char keyCode)
{
	if (KEYDOWN(m_currKeyState, keyCode))
	{
		return true;
	}

	return false;
}

//Ű�� ���ȴٰ� ���� �� true�� �����ϴ� �Լ�
bool InputClass::GetKeyPressedAndRelease(const unsigned char keyCode)
{
	if (KEYDOWN(m_prevKeyState, keyCode))
	{
		if (!KEYDOWN(m_currKeyState, keyCode))
		{
			return true;
		}
	}

	return false;
}

//Ű�� ���ٰ� ������ �� true�� �����ϴ� �Լ�
bool InputClass::GetKeyReleasedAndPress(const unsigned char keyCode)
{
	if (!KEYDOWN(m_prevKeyState, keyCode))
	{
		if (KEYDOWN(m_currKeyState, keyCode))
		{
			return true;
		}
	}

	return false;
}

//���콺 ���� ��ư�� Press �������� Ȯ���ϴ� �Լ�
bool InputClass::GetLeftMouseButtonPressed()
{
	if (KEYDOWN(m_mouseState.rgbButtons, 0))
	{
		return true;
	}

	return false;
}

//���콺 ���� ��ư�� Release �������� Ȯ���ϴ� �Լ�
bool InputClass::GetLeftMouseButtonReleased()
{
	if (!KEYDOWN(m_mouseState.rgbButtons, 0))
	{
		return true;
	}

	return false;
}

//Press ���¿��� Release ���·� ��ȯ �ƴ��� Ȯ���ϴ� �Լ�
bool InputClass::GetLeftMouseButtonPressedAndReleased()
{
	if (KEYDOWN(m_prevMouseState, 0))
	{
		if (!KEYDOWN(m_mouseState.rgbButtons, 0))
		{
			return true;
		}
	}

	return false;
}

//Release ���¿��� Press ���·� ��ȯ �ƴ��� Ȯ���ϴ� �Լ�
bool InputClass::GetLeftMouseButtonReleasedAndPressed()
{
	if (!KEYDOWN(m_prevMouseState, 0))
	{
		if (KEYDOWN(m_mouseState.rgbButtons, 0))
		{
			return true;
		}
	}

	return false;
}

//���콺 ������ ��ư�� ���ȴ��� üũ�ϴ� �Լ�
bool InputClass::GetRightMouseButtonPressed()
{
	if (KEYDOWN(m_mouseState.rgbButtons, 1))
	{
		return true;
	}

	return false;
}

//���콺 ������ ��ư�� ���ȴ��� üũ�ϴ� �Լ�
bool InputClass::GetRightMouseButtonReleased()
{
	if (!KEYDOWN(m_mouseState.rgbButtons, 1))
	{
		return true;
	}

	return false;
}

void InputClass::AddText(const wchar_t text)
{
	unsigned int len = wcslen(m_text);

	if (len < 255)//��ĭ ���ڸ��� ���� �� ����
	{
		wcscat_s(m_text, 256, &text);
	}

	return;
}

void InputClass::SetText(const wchar_t* text)
{
	unsigned int len = wcslen(text);

	if (len < 256)//�迭 ���̸� �ʰ����� ���� ���
	{
		wcscpy_s(m_text, text);
	}

	return;
}

void InputClass::RemoveLastChar()
{
	unsigned int len = wcslen(m_text);
	
	//����� ���ڰ� ���� ��� �Լ� ����
	if (len == 0)
	{
		return;
	}

	m_text[len - 1] = L'\0';

	return;
}

const wchar_t* InputClass::GetText()
{
	return m_text;
}