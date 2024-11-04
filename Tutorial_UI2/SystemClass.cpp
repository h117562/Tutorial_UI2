#include "Systemclass.h"

SystemClass::SystemClass()
{
	m_applicationClass = 0;
	m_inputClass = 0;
	m_frameTimer = 0;
}

SystemClass::~SystemClass()
{
}

SystemClass::SystemClass(const SystemClass& other)
{
}

bool SystemClass::Initialize()
{
	bool result;

	//���÷��� ���� �ʱ�ȭ
	InitializeDisplaySettings();

	//������ �ʱ�ȭ
	result = InitializeWindow();
	if (!result)
	{
		return false;
	}

	//FrameTimer ����
	m_frameTimer = new FrameTimer();
	if (!m_frameTimer)
	{
		return false;
	}

	//FrameTimer �ʱ�ȭ
	result = m_frameTimer->Initialize();
	if (!result)
	{
		return false;
	}

	//InputClass ����
	m_inputClass = new InputClass();
	if (!m_inputClass)
	{
		return false;
	}

	//InputClass �ʱ�ȭ
	result = m_inputClass->Initialize(m_hinstance, m_hwnd);
	if (!result)
	{
		return false;
	}

	//ApplicationClass ����
	m_applicationClass = new ApplicationClass();
	if (!m_applicationClass)
	{
		return false;
	}

	//ApplicationClass �ʱ�ȭ
	result = m_applicationClass->Initialize(m_hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

bool SystemClass::InitializeWindow()
{
	bool result;

	WNDCLASSEX wc;
	RECT winSize;//������ ������
	int posX, posY;//������ ��ġ

	//WndProc �Լ����� SystemClass�� �ν��Ͻ��� �����ϱ� ���� ���� ����
	ApplicationHandle = this;

	//���� ���α׷��� ���μ��� �ν��Ͻ� �ڵ��� ������
	m_hinstance = GetModuleHandle(NULL);

	//������ Ŭ���� �̸�
	m_applicationName = L"Tutorial Reflect";

	//������ Ŭ���� ����
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.hIconSm = wc.hIcon;

	//������ Ŭ���� ���
	RegisterClassEx(&wc);

	//ȭ�� �߾� ��ġ �˾Ƴ���
	posX = (GetSystemMetrics(SM_CXSCREEN) - SCREEN_WIDTH) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - SCREEN_HEIGHT) / 2;

	//������ ��ġ, ũ�� ����
	winSize.left = 0;
	winSize.top = 0;
	winSize.right = SCREEN_WIDTH;
	winSize.bottom = SCREEN_HEIGHT;

	//â��� �϶� 
	if (!FULL_SCREEN)
	{
		//â �׵θ�, ��ܹ� ũ�⸦ ������ ũ��� �缳��
		AdjustWindowRect(&winSize, WS_OVERLAPPEDWINDOW, FALSE);
	}

	//������ ����
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,//�Ϲ� �����쿡�� ũ�� ������ ����
		posX, posY, winSize.right - winSize.left, winSize.bottom - winSize.top, NULL, NULL, m_hinstance, NULL);

	//�����츦 Ȱ��ȭ�ϰ� ȭ�鿡 ���
	ShowWindow(m_hwnd, SW_SHOW);

	//�����츦 ���� ������ ����
	SetForegroundWindow(m_hwnd);

	//Ű���� ��Ŀ���� �ش� ������� ����
	SetFocus(m_hwnd);

	return true;
}

void SystemClass::InitializeDisplaySettings()
{
	bool result;

	//�ش� �ؽ�Ʈ ���Ϸ� ���� ���� ���� ������
	string filename = "../settings.txt";

	ifstream fin;

	//���� �б�
	fin.open(filename);
	result = fin.is_open();

	if (!result)//������ ���� ���
	{
		//���÷��� �⺻ ����
		FULL_SCREEN = false;
		VSYNC_ENABLED = true;
		SCREEN_WIDTH = 1200;
		SCREEN_HEIGHT = 800;
		SCREEN_NEAR = 0.1f;
		SCREEN_FAR = 1000.0f;
	}
	else
	{
		const int optionNum = 11;	//�ɼ� ����
		string options[optionNum];

		//�ɼ� ������ŭ �ؽ�Ʈ ���Ͽ��� ������ �޾ƿ�
		for (int i = 0; i < optionNum; i++)
		{
			getline(fin, options[i]);
		}

		for (string str : options)
		{
			result = SearchOptions(str, "FULL_SCREEN = ");
			if (result)
			{
				string temp = str.substr(str.find_last_of("FULL_SCREEN = "));
				auto value = StringToBool(temp);
				FULL_SCREEN = value;
				break;
			}
		}

		for (string str : options)
		{
			result = SearchOptions(str, "VSYNC_ENABLED = ");
			if (result)
			{
				string temp = str.substr(str.find_last_of("VSYNC_ENABLED = "));
				auto value = StringToBool(temp);
				VSYNC_ENABLED = value;
				break;
			}
		}

		for (string str : options)
		{
			result = SearchOptions(str, "SCREEN_WIDTH = ");
			if (result)
			{
				string temp = str.substr(str.find_last_of("SCREEN_WIDTH = "));
				auto value = stoi(temp);
				SCREEN_WIDTH = (unsigned int)value;
				break;
			}
		}

		for (string str : options)
		{
			result = SearchOptions(str, "SCREEN_HEIGHT = ");
			if (result)
			{
				string temp = str.substr(str.find_last_of("SCREEN_HEIGHT = "));
				auto value = stoi(temp);
				SCREEN_HEIGHT = (unsigned int)value;
				break;
			}
		}

		for (string str : options)
		{
			result = SearchOptions(str, "SCREEN_NEAR = ");
			if (result)
			{
				string temp = str.substr(str.find_last_of("SCREEN_NEAR = "));
				auto value = stof(temp);
				SCREEN_NEAR = (float)value;
				break;
			}
		}

		for (string str : options)
		{
			result = SearchOptions(str, "SCREEN_FAR = ");
			if (result)
			{
				string temp = str.substr(str.find_last_of("SCREEN_FAR = "));
				auto value = stof(temp);
				SCREEN_FAR = (float)value;
				break;
			}
		}

		//for (string str : options)
		//{
		//	result = SearchOptions(str, "Sample = ");
		//	if (result)
		//	{
		//		string temp = str.substr(str.find_last_of("Sample = "));//�޺κ��� ���ڿ��� �߶� ����
		//		bool value = StringToBool(temp);
		//		Sample = value;
		//		break;
		//	}
		//}
	}

	return;
}

void SystemClass::Shutdown()
{

	if (m_applicationClass)
	{
		m_applicationClass->Shutdown();
		delete m_applicationClass;
		m_applicationClass = 0;
	}

	if (m_inputClass)
	{
		m_inputClass->Shutdown();
		delete m_inputClass;
		m_inputClass = 0;
	}

	if (m_frameTimer)
	{
		delete m_frameTimer;
		m_frameTimer = 0;
	}

	//������ �ڵ� �ı�
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	//��ϵ� ������ Ŭ������ ����
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	//SystemClass �ڵ� �����͸� NULL�� �ʱ�ȭ
	ApplicationHandle = NULL;

	return;
}


void SystemClass::Run()
{
	MSG msg;
	bool done, result;


	//���� �ʱ�ȭ
	ZeroMemory(&msg, sizeof(MSG));


	done = false;
	while (!done)
	{
		//�޽��� ����Ȯ��
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//���� �޽��� Ȯ��
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			//���ᰡ �ƴϸ� ����Ŭ�ݺ�
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}

	}

	return;
}

bool SystemClass::Frame()
{
	bool result;

	m_frameTimer->Frame();

	result = m_inputClass->Frame();
	if (!result)
	{
		return false;
	}

	result = m_applicationClass->Frame(m_hwnd, m_inputClass, m_frameTimer);
	if (!result)
	{
		return false;
	}

	return true;
}

bool SystemClass::StringToBool(string target)
{
	bool result;

	result = target.find("true") != string::npos;
	if (result)
	{
		return true;
	}

	return false;
}

bool SystemClass::SearchOptions(string target, const char* str)
{
	bool result;

	result = target.find(str) != string::npos;
	if (result)
	{
		return true;
	}

	return false;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, msg, wparam, lparam);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}