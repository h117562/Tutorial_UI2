#include "D3DClass.h"

D3DClass::D3DClass()
{
	m_device = 0;
	m_deviceContext = 0;
	m_swapChain = 0;
	m_renderTargetView = 0;

	m_depthStencilState = 0;
	m_depthDisabledStencilState = 0;
	m_depthStencilView = 0;
	m_depthStencilBuffer = 0;

	m_alphaEnableBlendingState = 0;
	m_alphaDisableBlendingState = 0;

	m_rasterState = 0;
	m_rasterStateWireframe = 0;
}

D3DClass::~D3DClass()
{
}

D3DClass::D3DClass(const D3DClass &other)
{
}

bool D3DClass::Initialize(HWND hwnd)
{
	HRESULT result;

	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, numerator, denominator;
	unsigned long long stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_BLEND_DESC blendStateDesc;

	float fieldOfView, screenAspect;

	//�⺻ ���ΰ�ħ �� �ʱ�ȭ
	numerator = 60;		//����
	denominator = 1;	//�и�

	//DXGI ���丮 ����
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	//0��° ����͸� ������
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	//0��° �����ġ�� ������
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	//��� ��ġ�κ��� �ش� �ȼ� ������ ����ϴ� ���÷��� ��� ����Ʈ�� ã��
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	//��� ����Ʈ�� ������ �ʱ�ȭ
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	//�迭 �ʱ�ȭ �� MODE_DESC ����
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	//ȭ�� ũ��� ��ġ�ϴ� ��带 ã�� ���ΰ�ħ �󵵸� ����
	for (UINT i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == SCREEN_WIDTH)
		{
			if (displayModeList[i].Height == SCREEN_HEIGHT)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	//�ش� ����ͷκ��� ������ ������
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}
	
	//�׷��� ī���� �޸� ũ�⸦ ���� (MB)
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	//�׷��� ī���� �̸��� ����
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	//���� ������� �ڿ��� ��ȯ
	delete[] displayModeList;
	displayModeList = 0;

	adapterOutput->Release();
	adapterOutput = 0;

	adapter->Release();
	adapter = 0;

	factory->Release();
	factory = 0;


	//�޸� �ʱ�ȭ
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	//���� ü�� ����
	swapChainDesc.BufferCount = 2;						//�� ���� ����
	swapChainDesc.BufferDesc.Width = SCREEN_WIDTH;		//�� ���� �ʺ�
	swapChainDesc.BufferDesc.Height = SCREEN_HEIGHT;		//�� ���� ����
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//�ȼ� ����
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //�� ���� ��� ��� ����
	swapChainDesc.OutputWindow = hwnd;	//����� �������� Hwnd �ڵ� 
	swapChainDesc.SampleDesc.Count = 1;	//���� ���� ����(1�� ���� ���ø��� ��) ex)���� ���ø�, ��Ƽ ���ø�
	swapChainDesc.SampleDesc.Quality = 0;//ǰ�� ���� ����(0�� ���� ���ø��� ��)
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;//��ĵ���� ���� �⺻
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;//Scaling �⺻
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;//���� ����۸� ������ ���� �׸���
	swapChainDesc.Flags = 0;//�÷��� ����

	//���ΰ�ħ �� ����
	if (VSYNC_ENABLED)
	{
		//Vsync�� �������� ��� ������� ������ ����
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		//�ƴ϶�� ���� ����
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	//��üȭ�� ����
	if (FULL_SCREEN)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	//��� ���� ����
	featureLevel = D3D_FEATURE_LEVEL_11_1;

	//D3DDevice, ����ü�� �׸��� DeviceContext ����
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
	if (FAILED(result))
	{
		return false;
	}

	//�� ���� �����͸� ����
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	//���� Ÿ�� �� ����
	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	//������ ����
	backBufferPtr->Release();
	backBufferPtr = 0;


	//�޸� �ʱ�ȭ
	ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

	//���� ���� ����
	depthBufferDesc.Width = SCREEN_WIDTH;	//�ʺ�
	depthBufferDesc.Height = SCREEN_HEIGHT;	//����
	depthBufferDesc.MipLevels = 1;//Mipmap ���� 
	depthBufferDesc.ArraySize = 1;//�ؽ�ó ����
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//�ؽ�ó ����
	depthBufferDesc.SampleDesc.Count = 1;//�ȼ��� ���� ���� ����
	depthBufferDesc.SampleDesc.Quality = 0;//�̹��� ǰ�� ����
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;//�ؽ�ó�� �а� ���� ���
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;//���������� �������� ���ε� �÷���
	depthBufferDesc.CPUAccessFlags = 0;//CPU �׼��� ���� (0�� ������)
	depthBufferDesc.MiscFlags = 0;

	//���� ���� ����
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}


	//�޸� �ʱ�ȭ
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	//���� ���ٽ� State ����
	depthStencilDesc.DepthEnable = true;//���� �׽�Ʈ Ȱ��ȭ
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;//���� ���ٽ� ���ۿ� ���⸦ ��
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;//���� �� �Լ� - ���� �����Ͱ� ��� �����ͺ��� ������ �� ���

	depthStencilDesc.StencilEnable = true;//���ٽ� �׽�Ʈ Ȱ��ȭ
	depthStencilDesc.StencilReadMask = 0xFF;//�б� ����ũ �� - ��� 8��Ʈ
	depthStencilDesc.StencilWriteMask = 0xFF;//���� ����ũ �� - ��� 8��Ʈ

	//�ո� �ȼ��� ��� �۾��� ���ٽ�
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;//���ٽ� �׽�Ʈ ���� �� ������ ����
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;//���� �׽�Ʈ ���� �� ���ٽ� �� 1 ����
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;//�Ѵ� ���� �� ������ ����
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;//���ٽ� ������ �� �Լ�

	//�޸� �ȼ��� ��� �۾��� ���ٽ�
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;//���ٽ� ���� 1 ����
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;//�׻� ��

	//���� ���ٽ� State ����
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	//���� ���ٽ� State ����
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	//2D ���� ���ٽ� State ����
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;

	//�ո� �ȼ��� ��� �۾��� ���ٽ�
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//�޸� �ȼ��� ��� �۾��� ���ٽ�
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//2D ���� ���ٽ� ���� ����
	result = m_device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);
	if (FAILED(result))
	{
		return false;
	}

	{//�ſ��
		D3D11_DEPTH_STENCIL_DESC maskDesc;
		ZeroMemory(&maskDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

		maskDesc.DepthEnable = true;
		maskDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		maskDesc.DepthFunc = D3D11_COMPARISON_LESS;

		maskDesc.StencilEnable = true;
		maskDesc.StencilReadMask = 0xFF;
		maskDesc.StencilWriteMask = 0xFF;

		maskDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		maskDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		maskDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		maskDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		maskDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		maskDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		maskDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		maskDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		result = m_device->CreateDepthStencilState(&maskDesc, &m_maskState);
		if (FAILED(result))
		{
			return false;
		}


		D3D11_DEPTH_STENCIL_DESC mirrorDesc;
		ZeroMemory(&mirrorDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

		mirrorDesc.DepthEnable = true;
		mirrorDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		mirrorDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		mirrorDesc.StencilEnable = true;
		mirrorDesc.StencilReadMask = 0xFF;
		mirrorDesc.StencilWriteMask = 0xFF;

		mirrorDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		mirrorDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		mirrorDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		mirrorDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		mirrorDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		mirrorDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		mirrorDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		mirrorDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		result = m_device->CreateDepthStencilState(&mirrorDesc, &m_mirrorState);
		if (FAILED(result))
		{
			return false;
		}
	}


	//�޸� �ʱ�ȭ
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	//���� ���ٽ� �� ����
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	//���� ���ٽ� �� ����
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	//���� Ÿ�� ��� ���� ���ٽ� �並 ��� ���� ������ ���ο� ���ε�
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	//�⺻ �����Ͷ����� ���� 
	rasterDesc.FillMode = D3D11_FILL_SOLID;//�������� �׸�
	rasterDesc.CullMode = D3D11_CULL_NONE;//������ ����� �׸�
	rasterDesc.FrontCounterClockwise = false;//�ð� �������� �׷��� �������� �ո�
	rasterDesc.DepthBias = 0;//���� ���̾ ���� ��
	rasterDesc.DepthBiasClamp = 0.0f;//�ȼ��� �ִ� ���� ���̾
	rasterDesc.SlopeScaledDepthBias = 0.0f;//�ȼ��� ���⿡�� ��Į��
	rasterDesc.DepthClipEnable = true;//�Ÿ��� ���� Ŭ������ ����ϵ��� ����
	rasterDesc.ScissorEnable = false;//���� ���簢�� �ø��� ������� �ʵ��� ����
	rasterDesc.MultisampleEnable = false;//���� ���� ��Ƽ�ٸ���� ��� ����
	rasterDesc.AntialiasedLineEnable = false;//���� ��Ƽ�ٸ���� ������


	//�⺻ �����Ͷ����� ���� ����
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result))
	{
		return false;
	}

	//�⺻ �����Ͷ����� ���� ����
	m_deviceContext->RSSetState(m_rasterState);

	//���̾� ������ ��� �����Ͷ����� ����
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	//���̾� ������ ��� ���� ����
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterStateWireframe);
	if (FAILED(result))
	{
		return false;
	}

	ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));

	//DirectX�� Ŭ�� ���� ��ǥ�� ������ ��� ������ ������ �� �ֵ��� ����Ʈ�� ������
	m_viewport.Width = (float)SCREEN_WIDTH;
	m_viewport.Height = (float)SCREEN_HEIGHT;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;

	//����Ʈ ���ε�
	m_deviceContext->RSSetViewports(1, &m_viewport);

	//���� ��� ����
	fieldOfView = 3.141592654f / 4.0f;
	screenAspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

	//�⺻ ���� ��� ����
	m_worldMatrix = DirectX::XMMatrixIdentity();

	//Perspective Projection ��� ����
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, SCREEN_NEAR, SCREEN_FAR);

	//2D �������� ���� Orthographic Projection(���� ����) ��� ����
	m_orthoMatrix = DirectX::XMMatrixOrthographicLH((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, SCREEN_NEAR, SCREEN_FAR);

	//���� �ʱ�ȭ
	ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));

	//���� ���� ����(���� ����)
	blendStateDesc.AlphaToCoverageEnable = false;
	blendStateDesc.IndependentBlendEnable = false;
	blendStateDesc.RenderTarget[0].BlendEnable = true;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;				//SourceBlend ���� �ȼ��� ���� ����
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;		//DestinationBlend ������ �׷��� �ȼ��鿡 ���� ����
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	//���� ���� ���� ����
	result = m_device->CreateBlendState(&blendStateDesc, &m_alphaEnableBlendingState);
	if (FAILED(result))
	{
		return false;
	}

	//���� ��Ȱ�� ���� ����
	blendStateDesc.RenderTarget[0].BlendEnable = false;

	//���� ��Ȱ�� ���� ����
	result = m_device->CreateBlendState(&blendStateDesc, &m_alphaDisableBlendingState);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void D3DClass::Shutdown()
{
	if (m_alphaDisableBlendingState)
	{
		m_alphaDisableBlendingState->Release();
		m_alphaDisableBlendingState = 0;
	}

	if (m_alphaEnableBlendingState)
	{
		m_alphaEnableBlendingState->Release();
		m_alphaEnableBlendingState = 0;
	}

	if (m_mirrorState)
	{
		m_mirrorState->Release();
		m_mirrorState = 0;
	}

	if (m_maskState)
	{
		m_maskState->Release();
		m_maskState = 0;
	}

	if (m_depthDisabledStencilState)
	{
		m_depthDisabledStencilState->Release();
		m_depthDisabledStencilState = 0;
	}

	if (m_rasterStateWireframe)
	{
		m_rasterStateWireframe->Release();
		m_rasterStateWireframe = 0;
	}

	if (m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = 0;
	}

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = 0;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = 0;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = 0;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = 0;
	}

	return;
}

void D3DClass::BeginScene(float r, float g, float b, float a) 
{
	float color[4];

	//���۸� ä�� ������ ����
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;

	//�� ���� Ŭ����
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	//���� ���� Ŭ����
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void D3DClass::EndScene() 
{
	//�� ���۸� ȭ�鿡 ǥ��
	if (VSYNC_ENABLED)
	{
		//����ȭ
		m_swapChain->Present(1, 0);
	}
	else
	{
		//����ȭ ����
		m_swapChain->Present(0, 0);
	}
	
	return;
}

ID3D11Device* D3DClass::GetDevice() 
{
	return m_device;
}

ID3D11DeviceContext* D3DClass::GetDeviceContext()
{
	return m_deviceContext;
}

IDXGISwapChain* D3DClass::GetSwapChain() 
{
	return m_swapChain;
}

void D3DClass::GetProjectionMatrix(DirectX::XMMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
	return;
}

void D3DClass::GetWorldMatrix(DirectX::XMMATRIX& worldMatrix)
{
	worldMatrix = m_worldMatrix;
	return;
}

void D3DClass::GetOrthoMatrix(DirectX::XMMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
	return;
}

void D3DClass::GetVideoCardInfo(char* description, int& memory)
{
	strcpy_s(description, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
	return;
}

void D3DClass::ResetRenderTarget()
{
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
	m_deviceContext->RSSetViewports(1, &m_viewport);

	return;
}

void D3DClass::ResetDepthStencilState()
{
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
	return;
}

void D3DClass::TurnZBufferOff()
{
	m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
	return;
}

void D3DClass::TurnMirror()
{
	m_deviceContext->OMSetDepthStencilState(m_mirrorState, 1);
	return;
}

void D3DClass::TurnMask()
{
	m_deviceContext->OMSetDepthStencilState(m_maskState, 1);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_STENCIL, 1.0f, 0);
	return;
}

void D3DClass::EnableAlphaBlending()
{
	float blendFactor[4];

	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	m_deviceContext->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);

	return;
}

void D3DClass::DisableAlphaBlending()
{
	float blendFactor[4];

	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	m_deviceContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);

	return;
}

void D3DClass::EnableWireframe()
{
	//���̾� ������ ��� ����
	m_deviceContext->RSSetState(m_rasterStateWireframe);

	return;
}

void D3DClass::DisableWireframe() 
{
	//���̾� ������ ��� ����
	m_deviceContext->RSSetState(m_rasterState);

	return;
}

