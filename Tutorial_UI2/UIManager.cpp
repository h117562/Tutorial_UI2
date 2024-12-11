#include "UIManager.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

UIManager::UIManager(const UIManager& other)
{
}

bool UIManager::Initialize(D3DClass* pD3Dclass, TextClass* pTextClass)
{
	bool result;

	//���� ����� �������� ȭ�鿡 �������ϴ� UI Ŭ����
	result = m_debugUI.Initialize(pD3Dclass);
	if (!result)
	{
		return false;
	}

	EventClass::GetInstance().Subscribe(UI_EVENT::TOGGLE_DEBUG_MODE, [&]() {m_debugUI.ToggleActive(); });

	result = m_canvas.Initialize(pD3Dclass->GetDevice(), pTextClass);
	if (!result)
	{
		return false;
	}

	EventClass::GetInstance().Subscribe(UI_EVENT::TOGGLE_TEST_CANVAS, [&]() {m_canvas.ToggleActive(); });

	return true;
}


bool UIManager::Frame(D3DClass* pD3DClass, HWND hwnd, ShaderManager* pShaderManager, TextClass* pTextClass, CameraClass* pCameraClass)
{
	bool result, state;

	pD3DClass->TurnZBufferOff();

	if (m_debugUI.GetActive())
	{
		m_debugUI.Frame(pTextClass, pCameraClass);
	}

	if (m_canvas.GetActive())
	{
		m_canvas.Frame(pD3DClass, hwnd, pShaderManager, pTextClass, pCameraClass);
	}

	pD3DClass->TurnZBufferOn();

	return true;
}


void UIManager::Shutdown()
{
	return;
}