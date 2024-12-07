#include "DebugInfo.h"

DebugInfo::DebugInfo()
{
	m_fps = 0;
	m_count = 0;
	m_startTime = 0;
	m_active = true;

	bool m_active;
	int m_fps, m_count;
	wchar_t m_fpsString[20];
	wchar_t m_videoNameString[144];
	wchar_t m_memoryString[32];
	wchar_t m_tempString[16];
	unsigned long m_startTime;

	DirectX::XMINT3 m_position, m_rotation;
	DirectX::XMINT3 m_prevPos, m_prevRot;
	long m_mouseX, m_mouseY;
	long m_prevMX, m_prevMY;
	wchar_t m_stringPX[16], m_stringPY[16], m_stringPZ[16];
	wchar_t m_stringRX[16], m_stringRY[16], m_stringRZ[16];
	wchar_t m_stringMX[16], m_stringMY[16];
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

	//그래픽 카드 이름과, 메모리 크기를 가져옴
	d3dClass->GetVideoCardInfo(videoCard, videoMemory);

	//변수 초기화
	wcscpy_s(m_videoNameString, L"");
	MultiByteToWideChar(CP_ACP, 0, videoCard, strlen(videoCard) + 1, m_videoNameString, _countof(m_videoNameString));
	//wchar_t용 Itoa, 마지막 매개변수는 10진수를 뜻함 기본 16진수이므로 꼭 10 써줘야 함
	_itow_s(videoMemory, m_tempString, _countof(m_tempString), 10);

	wcscpy_s(m_memoryString, L"VRAM: ");
	wcscat_s(m_memoryString, m_tempString);
	wcscat_s(m_memoryString, L" MB");

	//변수 초기화
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

void DebugInfo::Frame(TextClass* pTextClass, CameraClass* pCameraClass, InputClass* pInputClass)
{

	pCameraClass->GetPosition(m_position);
	pCameraClass->GetRotation(m_rotation);
	pInputClass->GetMousePosition(m_mouseX, m_mouseY);

	GetFps();

	//FPS를 업데이트
	wcscpy_s(m_fpsString, L"FPS: ");
	_itow_s(m_fps, m_tempString, _countof(m_tempString), 10);
	wcscat_s(m_fpsString, m_tempString);

	//카메라 위치 업데이트
	//값 변동이 있을 경우에만 텍스트 초기화
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
		
	//업데이트된 문자열들을 출력
	Render(pTextClass);

	return;
}

void DebugInfo::Render(TextClass* pTextClass)
{
	//FPS 정보 출력
	pTextClass->RenderText(m_fpsString, 0, 0, 800, 400);

	//비디오 카드 정보를 출력
	pTextClass->RenderText(m_videoNameString, 0, 30, 800, 400);
	pTextClass->RenderText(m_memoryString, 0, 60, 800, 400);

	//카메라 위치 정보를 출력
	pTextClass->RenderText(m_stringPX, 0, 120, 800, 400);
	pTextClass->RenderText(m_stringPY, 0, 150, 800, 400);
	pTextClass->RenderText(m_stringPZ, 0, 180, 800, 400);
	pTextClass->RenderText(m_stringRX, 0, 210, 800, 400);
	pTextClass->RenderText(m_stringRY, 0, 240, 800, 400);
	pTextClass->RenderText(m_stringRZ, 0, 270, 800, 400);

	//마우스 좌표 출력
	pTextClass->RenderText(m_stringMX, 0, 300, 800, 400);
	pTextClass->RenderText(m_stringMY, 0, 330, 800, 400);
}

void DebugInfo::GetFps()
{
	m_count++;

	if (timeGetTime() >= (m_startTime + 1000))
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