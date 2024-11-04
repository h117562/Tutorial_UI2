#include "ApplicationClass.h"

ApplicationClass::ApplicationClass()
{
	m_Direct3D = 0;
	m_CameraClass = 0;
	m_uiManager = 0;
	m_TextClass = 0;
	m_ShaderManager = 0;

	m_rectangle = 0;
	m_mirror = 0;
}

ApplicationClass::~ApplicationClass()
{
}

ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}

bool ApplicationClass::Initialize(HWND hwnd)
{
	bool result;

	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	//DirectX �ʱ�ȭ
	result = m_Direct3D->Initialize(hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	m_ShaderManager = new ShaderManager;
	if (!m_ShaderManager)
	{
		return false;
	}

	//���̴� �Ŵ��� �ʱ�ȭ
	result = m_ShaderManager->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the ShaderManager.", L"Error", MB_OK);
		return false;
	}

	m_TextClass = new TextClass;
	if (!m_TextClass)
	{
		return false;
	}

	//TextClass �ʱ�ȭ
	result = m_TextClass->Initialize(m_Direct3D->GetSwapChain());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the TextClass.", L"Error", MB_OK);
		return false;
	}

	m_uiManager = new UIManager;
	if (!m_uiManager)
	{
		return false;
	}

	//UI �Ŵ��� �ʱ�ȭ
	result = m_uiManager->Initialize(m_Direct3D);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the UI Manager.", L"Error", MB_OK);
		return false;
	}

	m_CameraClass = new CameraClass;
	if (!m_CameraClass)
	{
		return false;
	}
	
	//�⺻ �� ��Ʈ���� �ʱ�ȭ
	m_CameraClass->SetBaseViewMatrix();

	//�ſ� �� �� �ʱ�ȭ
	{
		m_rectangle = new RectangleModel;
		if (!m_rectangle)
		{
			return false;
		}

		result = m_rectangle->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext());
		if (!result)
		{
			MessageBox(hwnd, L"Rect Initialize Failed", L"Error", MB_OK);
			return false;
		}

		m_mirror = new MirrorModel;
		if (!m_mirror)
		{
			return false;
		}

		result = m_mirror->Initialize(m_Direct3D->GetDevice());
		if (!result)
		{
			MessageBox(hwnd, L"Mirror Initialize Failed", L"Error", MB_OK);
			return false;
		}
	}

	return result;
}

void ApplicationClass::Shutdown()
{
	//�ſ�� �� ���� �޸� ����
	{
		if (m_mirror)
		{
			m_mirror->Shutdown();
			delete m_mirror;
			m_mirror = 0;
		}

		if (m_rectangle)
		{
			m_rectangle->Shutdown();
			delete m_rectangle;
			m_rectangle = 0;
		}
	}

	if (m_uiManager)
	{
		m_uiManager->Shutdown();
		delete m_uiManager;
		m_uiManager = 0;
	}

	if (m_TextClass)
	{
		m_TextClass->Shutdown();
		delete m_TextClass;
		m_TextClass = 0;
	}

	if (m_CameraClass)
	{
		delete m_CameraClass;
		m_CameraClass = 0;
	}

	if (m_ShaderManager)
	{
		m_ShaderManager->Shutdown();
		delete m_ShaderManager;
		m_ShaderManager = 0;
	}

	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	return;
}

bool ApplicationClass::Frame(HWND hwnd, InputClass* pInputClass, FrameTimer* pFrameTimer)
{
	bool result;

	//����� �Է� ó��
	HandleInput(pInputClass, pFrameTimer);

	//�׷��� ������
	Render(hwnd, pInputClass);

	return true;
}

void ApplicationClass::HandleInput(InputClass* pInputClass, FrameTimer* pFrameTimer)
{
	bool state;
	float deltaX, deltaY, frameTime;

	frameTime = pFrameTimer->GetTime();
	m_CameraClass->SetFrameTime(frameTime);

	state = pInputClass->GetKeyPressed(DIK_A);
	m_CameraClass->MoveLeft(state);

	state = pInputClass->GetKeyPressed(DIK_S);
	m_CameraClass->MoveBackward(state);

	state = pInputClass->GetKeyPressed(DIK_D);
	m_CameraClass->MoveRight(state);

	state = pInputClass->GetKeyPressed(DIK_W);
	m_CameraClass->MoveForward(state);

	pInputClass->GetMouseDelta(deltaX, deltaY);
	m_CameraClass->UpdateRotation(deltaX, deltaY);

	return;
}


void ApplicationClass::Render(HWND hwnd, InputClass* pInputClass)
{
	//3D RenderTarget �ʱ�ȭ(Ư�� �÷���)
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.2f, 1.0f);

	//2D RenderTarget �ʱ�ȭ
	m_TextClass->BeginDraw();

	{
		XMMATRIX modelWorld, view, proj;
		
		m_CameraClass->Render();
		m_Direct3D->GetProjectionMatrix(proj);
		m_CameraClass->GetViewMatrix(view);
		modelWorld = XMMatrixIdentity();

		//�������� �� �׸���
		m_ShaderManager->GetTextureShader()->Render(m_Direct3D->GetDeviceContext(), modelWorld, view, proj);
		m_rectangle->Render(m_Direct3D->GetDeviceContext());

		//����ŷ ������ �����ϱ� ���� ���ٽ� �������� ��ȯ
		m_Direct3D->TurnMask();

		//�ſ� ��ġ&ȸ�� ���� ��Ʈ����
		XMMATRIX mirrorWorld;

		//������ ��ġ�� ����
		mirrorWorld = XMMatrixIdentity() * XMMatrixRotationX(XM_PI/2) * XMMatrixTranslation(0.0f, -0.7f, 0.0f) * XMMatrixScaling(2,2,2);

		//�ſ� ������
		m_ShaderManager->GetColorShader()->Render(m_Direct3D->GetDeviceContext(), mirrorWorld, view, proj);
		m_mirror->Render(m_Direct3D->GetDeviceContext());

		//�ſ��� ���� 3��
		XMVECTOR pos1, pos2, pos3;

		pos1 = XMVectorSet(-1.0f, -1.0f, 0.0f, 0);
		pos2 = XMVectorSet(-1.0f, 1.0f, 0.0f, 0);
		pos3 = XMVectorSet(1.0f, -1.0f, 0.0f, 0);

		//�ſ��� ��ġ&ȸ���� ������ ����
		pos1 = XMVector3Transform(pos1, mirrorWorld);
		pos2 = XMVector3Transform(pos2, mirrorWorld);
		pos3 = XMVector3Transform(pos3, mirrorWorld);

		//������ ���� �� �������� ����
		XMVECTOR plane = XMPlaneFromPoints(
			pos1,
			pos2,
			pos3
		);

		//�ݻ� ����� �̿��Ͽ� ���� �ݻ�Ǵ� ��ġ�� ���
		XMMATRIX reflectMat = modelWorld * XMMatrixReflect(plane);
		
		//����ŷ �������� �߶� ȭ���� �������ϱ� ���� �ٽ� ���� ��ȯ
		m_Direct3D->TurnMirror();

		//�ݻ�� ��ġ&ȸ������ �ſ� ���������� ���� ������
		m_ShaderManager->GetTextureShader()->Render(m_Direct3D->GetDeviceContext(), reflectMat, view, proj);
		m_rectangle->Render(m_Direct3D->GetDeviceContext());

		//�ٽ� ���� ���ٽ� �������� �ǵ���
		m_Direct3D->ResetDepthStencilState();
	}

	//UI ������
	m_uiManager->Frame(m_Direct3D, hwnd, m_ShaderManager, m_TextClass, m_CameraClass, pInputClass);

	m_TextClass->EndDraw();
	m_Direct3D->EndScene();

	return;
}