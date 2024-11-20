#include "TestCanvas.h"

TestCanvas::TestCanvas()
{
}

TestCanvas::~TestCanvas()
{
}

bool TestCanvas::Initialize(ID3D11Device* pDevice)
{
	HRESULT result;

	m_btn1 = new Button;
	if (!m_btn1)
	{
		return false;
	}
	
	result = m_btn1->InitializeBuffer(pDevice);
	if (FAILED(result))
	{
		return false;
	}

	result = m_btn1->SetTexture1(pDevice, L"..//data//assets//mobile-button.png");
	if (FAILED(result))
	{
		return false;
	}

	result = m_btn1->SetTexture2(pDevice, L"..//data//assets//mobile-button-1.png");
	if (FAILED(result))
	{
		return false;
	}

	m_btn1->SetScale(XMFLOAT3(2.0f, 2.0f, 2.0f));
}

bool TestCanvas::Frame(D3DClass* pD3DClass, HWND hwnd, ShaderManager* pShaderManager, TextClass* pTextClass, CameraClass* pCameraClass, InputClass* pInputClass)
{
	bool result;
	
	POINT mpos;
	RECT rect;
	XMFLOAT2 npos;

	GetCursorPos(&mpos);
	ScreenToClient(hwnd, &mpos);
	GetClientRect(hwnd, &rect);

	npos = XMFLOAT2(
		((2.0f * static_cast<float>(mpos.x)) / static_cast<float>(rect.right - rect.left)) - 1.0f,
		((-2.0f * static_cast<float>(mpos.y)) / static_cast<float>(rect.bottom - rect.top) + 1.0f)
	);//마우스 위치를 정규화


	XMMATRIX view, proj;

	pCameraClass->GetBaseViewMatrix(view);
	pD3DClass->GetProjectionMatrix(proj);

	m_btn1->IsPressed(pInputClass, view, proj, npos.x, npos.y);

	result = m_btn1->Render(pD3DClass->GetDeviceContext(), pShaderManager->GetUIShader(), XMMatrixIdentity(), view, proj);
	if (!result)
	{
		return false;
	}

	return true;
}

void TestCanvas::SetActive(bool state)
{
	m_active = state;
}

bool TestCanvas::GetActive()
{
	return m_active;
}