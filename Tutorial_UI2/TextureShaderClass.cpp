#include "TextureShaderClass.h"

TextureShaderClass::TextureShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
	m_samplerState = 0;
	m_bufferCount = 1;
	m_startNumber = 0;
}

TextureShaderClass::~TextureShaderClass()
{
}

TextureShaderClass::TextureShaderClass(const TextureShaderClass& other)
{
}

bool TextureShaderClass::Initialize(ID3D11Device* pDevice, HWND hwnd)
{
	HRESULT result;
	ID3DBlob* vertexShaderBuffer;
	ID3DBlob* pixelShaderBuffer;
	ID3DBlob* errorMessage;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];

	unsigned int numElements;

	const wchar_t* vsFilename = L"../TextureVS.hlsl";
	const wchar_t* psFilename = L"../TexturePS.hlsl";

	//�ʱ�ȭ
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	//HLSL ������
	result = D3DCompileFromFile(vsFilename, NULL, NULL, "main", "vs_5_0",
		D3DCOMPILE_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);

	if (FAILED(result))
	{
		//���Ϸκ��� �����Ͽ� �������� ��� ������ �޽��� �ڽ��� ����
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd);
		}
		//���� ���� �޽����� ���� ��� ������ ã�� �� ���ٴ� �ǹ�
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Vs Texture Shader File", MB_OK);
		}

		return false;
	}

	//HLSL ������
	result = D3DCompileFromFile(psFilename, NULL, NULL, "main", "ps_5_0",
		D3DCOMPILE_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);

	if (FAILED(result))
	{
		//���Ϸκ��� �����Ͽ� �������� ��� ������ �޽��� �ڽ��� ����
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd);
		}
		//���� ���� �޽����� ���� ��� ������ ã�� �� ���ٴ� �ǹ�
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Ps Texture Shader File", MB_OK);
		}

		return false;
	}

	//���� ���̴� ���� (������ �� ���̴� ����)
	result = pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	//�ȼ� ���̴� ���� (������ �� ���̴� ����)
	result = pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	polygonLayout[0].SemanticName = "POSITION";//Semantic ���
	polygonLayout[0].SemanticIndex = 0;//���� Semantic ��� ���� ��ȣ
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;//��� ������ ����
	polygonLayout[0].InputSlot = 0;//0~15
	polygonLayout[0].AlignedByteOffset = 0;//��Ұ� ���۵Ǵ� ��ġ(���۾� �������� ����)
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;//������ ������
	polygonLayout[0].InstanceDataStepRate = 0;//D3D11_INPUT_PER_VERTEX_DATA�� 0���� ����

	polygonLayout[1].SemanticName = "TEXCOORD";//Semantic ���
	polygonLayout[1].SemanticIndex = 0;//���� Semantic ��� ���� ��ȣ
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;//��� ������ ����
	polygonLayout[1].InputSlot = 0;//0~15
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;//�ڵ� ���
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;//������ ������
	polygonLayout[1].InstanceDataStepRate = 0;//D3D11_INPUT_PER_VERTEX_DATA�� 0���� ����

	//�Է� �������� ����
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//���� �Է� ���̾ƿ� ����
	result = pDevice->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	//���̴� ���� ����
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	
	D3D11_BUFFER_DESC matrixBufferDesc;

	ZeroMemory(&matrixBufferDesc, sizeof(D3D11_BUFFER_DESC));

	//��Ʈ���� ���� ����
	matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);//���� ũ��
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;//GPU(�б� ����)�� CPU(���� ����)���� �׼��� ���� (�� ������ ���� ������Ʈ)
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//���۸� ��� ���۷� ���ε�
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//CPU �׼��� ����
	matrixBufferDesc.MiscFlags = 0;//���ҽ��� ���� �÷���
	matrixBufferDesc.StructureByteStride = 0;

	//��Ʈ���� ���� ����
	result = pDevice->CreateBuffer(&matrixBufferDesc, nullptr, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_SAMPLER_DESC sampDesc;

	//�޸� �ʱ�ȭ
	ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));

	//Sampler State ����
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;//�ؽ�ó ���͸� ��� (���� ����)
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;//�ؽ�ó ��ǥ�� 0���� 1���̸� �ݺ��ϴ� ����� ������
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;//�� ��쿡�� 1�� ������ �������� ��ǥ�� ���
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;//D3D11_TEXTURE_ADDRESS_CLAMP�� �ʰ��ϰų� �̸��� ��� 0 �Ǵ� 1�� ����
	sampDesc.MipLODBias = 0.0f;//Mipmap ������
	sampDesc.MaxAnisotropy = 1;//1~16
	sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;//�׻� �� ��� (��� �񱳰� true)
	sampDesc.MinLOD = 0;//�ּ� Mipmap ����
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;//�ִ� Mipmap ����

	//Sampler State ����
	result = pDevice->CreateSamplerState(&sampDesc, &m_samplerState);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool TextureShaderClass::Render(ID3D11DeviceContext* pDeviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	bool result;

	//���� ������Ʈ
	result = UpdateShaderBuffers(pDeviceContext, worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);//���� ����(�ﰢ��)
	pDeviceContext->IASetInputLayout(m_layout);//�Է� ���̾ƿ� ���ε�
	pDeviceContext->VSSetShader(m_vertexShader, NULL, 0);
	pDeviceContext->PSSetShader(m_pixelShader, NULL, 0);
	pDeviceContext->PSSetSamplers(0, 1, &m_samplerState);

	return true;
}

void TextureShaderClass::Shutdown()
{
	if (m_samplerState)
	{
		m_samplerState->Release();
		m_samplerState = 0;
	}

	//��Ʈ���� ���� ����
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	//���̾ƿ� ����
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	//�ȼ� ���̴� ����
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	//���� ���̴� ����
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	return;
}


//���� ������ �޽��� �ڽ��� ���
void TextureShaderClass::OutputShaderErrorMessage(ID3DBlob* errorMessage, HWND hwnd)
{
	char* compileErrors;

	//���� �޽��� �ؽ�Ʈ ���ۿ� ���� �����͸� ������
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	//�޽��� �ڽ� ����
	MessageBoxA(hwnd, compileErrors, "Error", MB_OK);

	//����
	errorMessage->Release();
	errorMessage = 0;

	return;
}

//���̴����� ����ϴ� ���۸� ������Ʈ
bool TextureShaderClass::UpdateShaderBuffers(ID3D11DeviceContext* pDeviceContext,
	XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBuffer* dataPtr = 0;

	//��� ��ġ
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	//��� ���۸� ��� (GPU �׼��� ��Ȱ��ȭ) 
	result = pDeviceContext->Map(m_matrixBuffer, NULL, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	//���� ���̴��� ��� ������ �����͸� ����
	dataPtr = (MatrixBuffer*)mappedResource.pData;

	//�Ű������� ������ �����ͷ� ������Ʈ
	dataPtr->worldMatrix = worldMatrix;
	dataPtr->viewMatrix = viewMatrix;
	dataPtr->projectionMatrix = projectionMatrix;

	//��� ���۸� ��� ���� (GPU �׼��� �ٽ� Ȱ��ȭ)
	pDeviceContext->Unmap(m_matrixBuffer, NULL);

	//���� ���̴��� ��� ���۸� ������Ʈ
	pDeviceContext->VSSetConstantBuffers(m_startNumber, m_bufferCount, &m_matrixBuffer);

	return true;
}