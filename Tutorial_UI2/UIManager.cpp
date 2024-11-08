#include "UIManager.h"

UIManager::UIManager()
{
	m_debugUI = 0;

	m_enableDebugUI = true;
}

UIManager::~UIManager()
{
}

UIManager::UIManager(const UIManager& other)
{
}

bool UIManager::Initialize(D3DClass* pD3Dclass)
{
	bool result;

	//각종 디버깅 정보들을 화면에 렌더링하는 UI 클래스
	m_debugUI = new DebugInfo;
	if (!m_debugUI)
	{
		return false;
	}

	result = m_debugUI->Initialize(pD3Dclass);
	if (!result)
	{
		return false;
	}


	return true;
}


void UIManager::Frame(D3DClass* pD3DClass, HWND hwnd, ShaderManager* pShaderManager, TextClass* pTextClass, CameraClass* pCameraClass, InputClass* pInputClass)
{
	bool state;

	//F5 버튼을 눌러 UI를 온오프
	state = pInputClass->GetKeyReleasedAndPress(DIK_F5);
	if (state)
	{
		m_enableDebugUI ^= true; //bool 형 반전
	}

	if (m_enableDebugUI)
	{
		m_debugUI->Render(pTextClass, pCameraClass);
	}

	return;
}


void UIManager::Shutdown()
{
	if (m_debugUI)
	{
		m_debugUI->Shutdown();
		delete m_debugUI;
		m_debugUI = 0;
	}

	return;
}