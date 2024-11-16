#include "RectangleModel.h"

RectangleModel::RectangleModel()
{
	m_vertices = 0;
	m_indices = 0;
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_resource = 0;
}

RectangleModel::~RectangleModel()
{
}

RectangleModel::RectangleModel(const RectangleModel&)
{
}

bool RectangleModel::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	HRESULT result;

	m_vertices = new VertexUV[6];
	m_indices = new UINT[6];

	m_vertices[0].position = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f);
	m_vertices[0].textureCoord = DirectX::XMFLOAT2(0.0f, 1.0f); // 왼쪽 아래

	m_vertices[1].position = DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f);
	m_vertices[1].textureCoord = DirectX::XMFLOAT2(0.0f, 0.0f); // 왼쪽 위

	m_vertices[2].position = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f);
	m_vertices[2].textureCoord = DirectX::XMFLOAT2(1.0f, 1.0f); // 오른쪽 아래

	m_vertices[3].position = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);
	m_vertices[3].textureCoord = DirectX::XMFLOAT2(1.0f, 0.0f); // 오른쪽 위

	m_vertices[4].position = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f);
	m_vertices[4].textureCoord = DirectX::XMFLOAT2(1.0f, 1.0f); // 오른쪽 아래

	m_vertices[5].position = DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f);
	m_vertices[5].textureCoord = DirectX::XMFLOAT2(0.0f, 0.0f); // 왼쪽 위

	m_indices[0] = 0;
	m_indices[1] = 1;
	m_indices[2] = 2;
	m_indices[3] = 3;
	m_indices[4] = 4;
	m_indices[5] = 5;

	//인덱스의 갯수
	m_indexCount = 6;

	//정점 버퍼에 대한 설명을 입력한다.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexUV) * m_indexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = m_vertices;

	//Desc를 기반으로 버퍼를 생성한다.
	result = pDevice->CreateBuffer(&vertexBufferDesc, &initData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//인덱스 버퍼에 대한 설명을 입력한다.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	initData.pSysMem = &m_indices[0];


	//Desc를 기반으로 버퍼를 생성한다.
	result = pDevice->CreateBuffer(&indexBufferDesc, &initData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	const wchar_t* filename = L"../data/sampleTex.png";//텍스처 경로
	DirectX::ScratchImage img;

	result = DirectX::LoadFromWICFile(filename, DirectX::WIC_FLAGS_NONE, nullptr, img);
	if (FAILED(result)) 
	{
		return false;
	}

	result = DirectX::CreateTextureEx(pDevice, 
		img.GetImages(), 
		img.GetImageCount(), 
		img.GetMetadata(), 
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
		0,
		0,
		DirectX::CREATETEX_DEFAULT,
		&m_resource);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shaderResourceViewDesc.Format = img.GetMetadata().format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	result = pDevice->CreateShaderResourceView(m_resource, &shaderResourceViewDesc, &m_resourceView);
	if (FAILED(result))
	{
		m_resourceView = NULL;
	}

	return true;
}


void RectangleModel::Render(ID3D11DeviceContext* pDeviceContext)
{
	pDeviceContext->PSSetShaderResources(0, 1, &m_resourceView);
	pDeviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	pDeviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pDeviceContext->DrawIndexed(m_indexCount, 0, 0);
}



void RectangleModel::Shutdown()
{

	if (m_resourceView)
	{
		m_resourceView->Release();
		m_resourceView = 0;
	}

	if (m_resource)
	{
		m_resource->Release();
		m_resource = 0;
	}

	//정점, 인덱스 버퍼 해제
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if (m_vertices)
	{
		delete[] m_vertices;
		m_vertices = 0;
	}

	if (m_indices)
	{
		delete[] m_indices;
		m_indices = 0;
	}

	return;
}