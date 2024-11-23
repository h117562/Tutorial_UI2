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

	const wchar_t* filepath[2] = {
		L"..//data//assets//mobile-button.png",
		L"..//data//assets//mobile-button-1.png"
	};

	result = m_btn1->SetTexture(pDevice, filepath, 2);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool TestCanvas::Frame(D3DClass* pD3DClass, HWND hwnd, ShaderManager* pShaderManager, TextClass* pTextClass, CameraClass* pCameraClass, InputClass* pInputClass)
{
	bool result;
	
	//POINT mpos;
	//RECT rect;
	//XMFLOAT2 npos;

	//GetCursorPos(&mpos);
	//ScreenToClient(hwnd, &mpos);
	//GetClientRect(hwnd, &rect);

	//npos = XMFLOAT2(
	//	((2.0f * static_cast<float>(mpos.x)) / static_cast<float>(rect.right - rect.left)) - 1.0f,
	//	((-2.0f * static_cast<float>(mpos.y)) / static_cast<float>(rect.bottom - rect.top) + 1.0f)
	//);//마우스 위치를 정규화

	XMMATRIX world, view, proj;

	world = XMMatrixIdentity()*XMMatrixScaling(500,500,100);
	pCameraClass->GetBaseViewMatrix(view);
	pD3DClass->GetOrthoMatrix(proj);

	//m_btn1->IsPressed(pInputClass, view, proj, npos.x, npos.y);

	result = m_btn1->Render(pD3DClass->GetDeviceContext(), pShaderManager->GetUIShader(), world, view, proj);
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