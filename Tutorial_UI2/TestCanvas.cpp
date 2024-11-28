#include "TestCanvas.h"

TestCanvas::TestCanvas()
{
	m_btn = 0;
	m_plane = 0;
	m_active = false;
}

TestCanvas::~TestCanvas()
{
	if (m_btn)
	{
		delete[] m_btn;
		m_btn = nullptr;
	}

	if (m_plane)
	{
		delete m_plane;
		m_plane = nullptr;
	}
}

bool TestCanvas::Initialize(ID3D11Device* pDevice)
{
	HRESULT result;
	
	m_btn = new Button[4];
	if (!m_btn)
	{
		return false;
	}
	
	for (int i = 0; i< 4; i++)
	{
		result = m_btn[i].Initialize(pDevice, 
			L"..//data//assets//mobile-button.png",
			L"..//data//assets//mobile-button-1.png",
			L"..//data//assets//mobile-button.png"
		);
		if (FAILED(result))
		{
			return false;
		}
	}

	m_btn[0].SetScale(200, 100, 100);
	m_btn[0].SetAlign(ALIGNMENT_LEFT);
	m_btn[0].UpdateWorldMatrix(SCREEN_WIDTH, SCREEN_HEIGHT);

	m_btn[1].SetScale(100, 100, 100);
	m_btn[1].SetAlign(ALIGNMENT_RIGHT);
	m_btn[1].UpdateWorldMatrix(SCREEN_WIDTH, SCREEN_HEIGHT);

	m_btn[2].SetScale(100, 200, 100);
	m_btn[2].SetAlign(ALIGNMENT_TOP);
	m_btn[2].UpdateWorldMatrix(SCREEN_WIDTH, SCREEN_HEIGHT);

	m_btn[3].SetScale(100, 100, 100);
	m_btn[3].SetAlign(ALIGNMENT_BOTTOM);
	m_btn[3].UpdateWorldMatrix(SCREEN_WIDTH, SCREEN_HEIGHT);

	m_plane = new Plane;
	if (!m_plane)
	{
		return false;
	}

	result = m_plane->Initialize(pDevice, L"..//data//assets//panel-2.png");
	if (FAILED(result))
	{
		return false;
	}

	m_plane->SetScale(500, 500, 500);
	m_plane->UpdateWorldMatrix(SCREEN_WIDTH, SCREEN_HEIGHT);

	return true;
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
	pD3DClass->GetOrthoMatrix(proj);

	for (int i = 0; i < 4; i++)
	{
		m_btn[i].Frame(pInputClass, view, proj, npos.x, npos.y);
	}


	result = m_btn[0].IsPressed();
	if (result)
	{
		//TODO
		MessageBox(hwnd, L"왼쪽 버튼 눌림", L"Check", MB_OK);
	}

	result = m_btn[1].IsPressed();
	if (result)
	{
		//TODO
		MessageBox(hwnd, L"오른쪽 버튼 눌림", L"Check", MB_OK);
	}

	result = m_btn[2].IsPressed();
	if (result)
	{
		//TODO
		MessageBox(hwnd, L"상단 버튼 눌림", L"Check", MB_OK);
	}

	result = m_btn[3].IsPressed();
	if (result)
	{
		//TODO
		MessageBox(hwnd, L"하단 버튼 눌림", L"Check", MB_OK);
	}


	result = Render(pD3DClass, pShaderManager, view, proj);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TestCanvas::Render(D3DClass* pD3DClass,ShaderManager* pShaderManager, const XMMATRIX& view, const XMMATRIX& proj)
{
	bool result;

	result = m_plane->Render(pD3DClass->GetDeviceContext(), pShaderManager->GetUIShader(), m_plane->GetWorldMatrix(), view, proj);
	if (!result)
	{
		return false;
	}

	for (int i = 0; i < 4; i++)
	{
		result = m_btn[i].Render(pD3DClass->GetDeviceContext(), pShaderManager->GetUIShader(), m_btn[i].GetWorldMatrix(), view, proj);
		if (!result)
		{
			return false;
		}
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