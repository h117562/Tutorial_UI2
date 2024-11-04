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

	//���� ����� �������� ȭ�鿡 �������ϴ� UI Ŭ����
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

	//F5 ��ư�� ���� UI�� �¿���
	state = pInputClass->GetKeyReleasedAndPress(DIK_F5);
	if (state)
	{
		m_enableDebugUI ^= true; //bool �� ����
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