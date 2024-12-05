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

bool UIManager::Initialize(D3DClass* pD3Dclass)
{
	bool result;

	//각종 디버깅 정보들을 화면에 렌더링하는 UI 클래스
	result = m_debugUI.Initialize(pD3Dclass);
	if (!result)
	{
		return false;
	}

	EventClass::GetInstance().Subscribe(UI_EVENT::EVENT_TOGGLE_DEBUG_MODE, [&]() {m_debugUI.ToggleActive(); });

	result = m_canvas.Initialize(pD3Dclass->GetDevice());
	if (!result)
	{
		return false;
	}

	EventClass::GetInstance().Subscribe(UI_EVENT::EVENT_TOGGLE_TEST_CANVAS, [&]() {m_canvas.ToggleActive(); });

	return true;
}


bool UIManager::Frame(D3DClass* pD3DClass, HWND hwnd, ShaderManager* pShaderManager, TextClass* pTextClass, CameraClass* pCameraClass, InputClass* pInputClass)
{
	bool result, state;

	pD3DClass->TurnZBufferOff();

	if (m_debugUI.GetActive())
	{
		m_debugUI.Frame(pTextClass, pCameraClass, pInputClass);
	}

	if (m_canvas.GetActive())
	{
		m_canvas.Frame(pD3DClass, hwnd, pShaderManager, pTextClass, pCameraClass, pInputClass);
	}

	pD3DClass->TurnZBufferOn();

	return true;
}


void UIManager::Shutdown()
{
	return;
}