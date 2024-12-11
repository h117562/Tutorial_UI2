#include "DebugInfo.h"

DebugInfo::DebugInfo()
{
	m_fps = 0;
	m_count = 0;
	m_startTime = 0;
	m_active = true;
}

DebugInfo::~DebugInfo()
{
}

DebugInfo::DebugInfo(const DebugInfo&)
{
}

bool DebugInfo::Initialize(D3DClass* d3dClass)
{
	bool result;
	char videoCard[128];
	int videoMemory;

	//�׷��� ī�� �̸���, �޸� ũ�⸦ ������
	d3dClass->GetVideoCardInfo(videoCard, videoMemory);

	//���� �ʱ�ȭ
	wcscpy_s(m_videoNameString, L"");
	MultiByteToWideChar(CP_ACP, 0, videoCard, strlen(videoCard) + 1, m_videoNameString, _countof(m_videoNameString));
	//wchar_t�� Itoa, ������ �Ű������� 10������ ���� �⺻ 16�����̹Ƿ� �� 10 ����� ��
	_itow_s(videoMemory, m_tempString, _countof(m_tempString), 10);

	wcscpy_s(m_memoryString, L"VRAM: ");
	wcscat_s(m_memoryString, m_tempString);
	wcscat_s(m_memoryString, L" MB");

	//���� �ʱ�ȭ
	m_prevPos = DirectX::XMINT3(0, 0, 0);
	m_prevRot = DirectX::XMINT3(0, 0, 0);

	wcscpy_s(m_stringPX, L"PX: 0");
	wcscpy_s(m_stringPY, L"PY: 0");
	wcscpy_s(m_stringPZ, L"PZ: 0");

	wcscpy_s(m_stringRX, L"RX: 0");
	wcscpy_s(m_stringRY, L"RY: 0");
	wcscpy_s(m_stringRZ, L"RZ: 0");

	wcscpy_s(m_stringMX, L"MouseX: 0");
	wcscpy_s(m_stringMY, L"MouseY: 0");

	return true;
}

void DebugInfo::Frame(TextClass* pTextClass, CameraClass* pCameraClass)
{

	pCameraClass->GetPosition(m_position);
	pCameraClass->GetRotation(m_rotation);
	InputClass::GetInstance().GetMousePosition(m_mouseX, m_mouseY);

	GetFps();

	//FPS�� ������Ʈ
	wcscpy_s(m_fpsString, L"FPS: ");
	_itow_s(m_fps, m_tempString, _countof(m_tempString), 10);
	wcscat_s(m_fpsString, m_tempString);

	//ī�޶� ��ġ ������Ʈ
	//�� ������ ���� ��쿡�� �ؽ�Ʈ �ʱ�ȭ
	if (m_position.x != m_prevPos.x)
	{
		wcscpy_s(m_stringPX, L"PX: ");
		_itow_s(m_position.x, m_tempString, _countof(m_tempString), 10);
		wcscat_s(m_stringPX, m_tempString);
		m_prevPos.x = m_position.x;
	}

	if (m_position.y != m_prevPos.y)
	{
		wcscpy_s(m_stringPY, L"PY: ");
		_itow_s(m_position.y, m_tempString, _countof(m_tempString), 10);
		wcscat_s(m_stringPY, m_tempString);
		m_prevPos.y = m_position.y;
	}

	if (m_position.z != m_prevPos.z)
	{
		wcscpy_s(m_stringPZ, L"PZ: ");
		_itow_s(m_position.z, m_tempString, _countof(m_tempString), 10);
		wcscat_s(m_stringPZ, m_tempString);
		m_prevPos.z = m_position.z;
	}

	if (m_rotation.x != m_prevRot.x)
	{
		wcscpy_s(m_stringRX, L"RX: ");
		_itow_s(m_rotation.x, m_tempString, _countof(m_tempString), 10);
		wcscat_s(m_stringRX, m_tempString);
		m_rotation.x = m_prevRot.x;
	}

	if (m_rotation.y != m_prevRot.y)
	{
		wcscpy_s(m_stringRY, L"RY: ");
		_itow_s(m_rotation.y, m_tempString, _countof(m_tempString), 10);
		wcscat_s(m_stringRY, m_tempString);
		m_rotation.y = m_prevRot.y;
	}

	if (m_rotation.z != m_prevRot.z)
	{
		wcscpy_s(m_stringRZ, L"RZ: ");
		_itow_s(m_rotation.z, m_tempString, _countof(m_tempString), 10);
		wcscat_s(m_stringRZ, m_tempString);
		m_rotation.z = m_prevRot.z;
	}

	if (m_mouseX != m_prevMX)
	{
		wcscpy_s(m_stringMX, L"MouseX: ");
		_ltow_s(m_mouseX, m_tempString, _countof(m_tempString), 10);
		wcscat_s(m_stringMX, m_tempString);
	}

	if (m_mouseY != m_prevMY)
	{
		wcscpy_s(m_stringMY, L"MouseY: ");
		_ltow_s(m_mouseY, m_tempString, _countof(m_tempString), 10);
		wcscat_s(m_stringMY, m_tempString);
	}
		
	//������Ʈ�� ���ڿ����� ���
	Render(pTextClass);

	return;
}

void DebugInfo::Render(TextClass* pTextClass)
{
	//FPS ���� ���
	pTextClass->RenderText(m_fpsString, D2D1::RectF(0.0f, 0.0f, 800.0f, 400.0f));

	//���� ī�� ������ ���
	pTextClass->RenderText(m_videoNameString, D2D1::RectF(0.0f, 30.0f, 800.0f, 400.0f));
	pTextClass->RenderText(m_memoryString, D2D1::RectF(0.0f, 60.0f, 800.0f, 400.0f));

	//ī�޶� ��ġ ������ ���
	pTextClass->RenderText(m_stringPX, D2D1::RectF(0.0f, 120.0f, 800.0f, 400.0f));
	pTextClass->RenderText(m_stringPY, D2D1::RectF(0.0f, 150.0f, 800.0f, 400.0f));
	pTextClass->RenderText(m_stringPZ, D2D1::RectF(0.0f, 180.0f, 800.0f, 400.0f));
	pTextClass->RenderText(m_stringRX, D2D1::RectF(0.0f, 210.0f, 800.0f, 400.0f));
	pTextClass->RenderText(m_stringRY, D2D1::RectF(0.0f, 240.0f, 800.0f, 400.0f));
	pTextClass->RenderText(m_stringRZ, D2D1::RectF(0.0f, 270.0f, 800.0f, 400.0f));

	//���콺 ��ǥ ���
	pTextClass->RenderText(m_stringMX, D2D1::RectF(0.0f, 300.0f, 800.0f, 400.0f));
	pTextClass->RenderText(m_stringMY, D2D1::RectF(0.0f, 330.0f, 800.0f, 400.0f));
}

void DebugInfo::GetFps()
{
	m_count++;

	if (timeGetTime() >= (m_startTime + 1000)) //winmm.lib �ʿ�
	{
		m_fps = m_count;
		m_count = 0;

		m_startTime = timeGetTime();
	}

	return;
}

void DebugInfo::ToggleActive()
{
	m_active ^= true;
}

void DebugInfo::SetActive(bool state)
{
	m_active = state;
}

bool DebugInfo::GetActive()
{
	return m_active;
}