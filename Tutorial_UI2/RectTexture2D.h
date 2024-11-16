#ifndef _RECT_TEXTURE_2D_
#define _RECT_TEXTURE_2D_

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "../data/DirectXTex.lib")

#include <d3d11.h>
#include <DirectXMath.h>

#include "../data/DirectXTex.h"
#include "GraphicStructures.h"

class RectTexture2D
{
public:
	RectTexture2D()
	{
		 m_vertexBuffer = 0;
		 m_indexBuffer = 0;
		 m_resource = 0;
		 m_resourceView = 0;
		 m_indexCount = 6;
		 m_stride = sizeof(VertexUV);
		 m_offset = 0;

		//��ȯ���� �ٷ� ����ĳ��Ʈ�� ����ϱ� ���ؼ� XMVECTOR�� ����
		m_vertices = new DirectX::XMVECTOR[4];

		m_vertices[0] = DirectX::XMVectorSet(-0.5f, 0.5f, 0.0f, 1.0f);//�»��
		m_vertices[1] = DirectX::XMVectorSet(0.5f, 0.5f, 0.0f, 1.0f);//����
		m_vertices[2] = DirectX::XMVectorSet(-0.5f, -0.5f, 0.0f, 1.0f);//���ϴ�
		m_vertices[3] = DirectX::XMVectorSet(0.5f, -0.5f, 0.0f, 1.0f);//���ϴ�

		m_indices = new UINT[m_indexCount];

		m_indices[0] = 0;
		m_indices[1] = 3;
		m_indices[2] = 2;
		m_indices[3] = 3;
		m_indices[4] = 0;
		m_indices[5] = 1;

	}

	~RectTexture2D()
	{
		if (m_indexBuffer)
		{
			m_indexBuffer->Release();
			m_indexBuffer = nullptr;
		}

		if (m_vertexBuffer)
		{
			m_vertexBuffer->Release();
			m_vertexBuffer = nullptr;
		}

		if (m_resource)
		{
			m_resource->Release();
			m_resource = nullptr;
		}

		if (m_resourceView)
		{
			m_resourceView->Release();
			m_resourceView = nullptr;
		}

		if (m_vertices)
		{
			delete[] m_vertices;
			m_vertices = nullptr;
		}

		if (m_indices)
		{
			delete[] m_indices;
			m_indices = nullptr;
		}
	}

	virtual HRESULT InitializeBuffer(ID3D11Device* pDevice)
	{
		HRESULT result;

		//���� ���� ����
		D3D11_BUFFER_DESC vertexBufferDesc;
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VertexUV) * m_indexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = m_vertices;

		//���� ���� ����
		result = pDevice->CreateBuffer(&vertexBufferDesc, &initData, &m_vertexBuffer);
		if (FAILED(result))
		{
			return result;
		}

		//�ε��� ���� ����
		D3D11_BUFFER_DESC indexBufferDesc;
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(UINT) * m_indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;
		initData.pSysMem = &m_indices[0];


		//�ε��� ���� ����
		result = pDevice->CreateBuffer(&indexBufferDesc, &initData, &m_indexBuffer);
		if (FAILED(result))
		{
			return result;
		}

		return S_OK;
	}

	virtual HRESULT SetTexture(ID3D11Device* pDevice, const wchar_t* filePath)
	{
		HRESULT result;

		DirectX::ScratchImage image;

		//�̹��� �����͸� �ε�
		result = DirectX::LoadFromWICFile(filePath, DirectX::WIC_FLAGS_NONE, nullptr, image);
		if (FAILED(result))
		{
			return result;
		}

		//���� �̹����� �����ͷ� �ؽ��� ���ҽ� ����
		result = DirectX::CreateTextureEx(pDevice,
			image.GetImages(),
			image.GetImageCount(),
			image.GetMetadata(),
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
			0,
			0,
			DirectX::CREATETEX_DEFAULT,
			&m_resource);

		if (FAILED(result))
		{
			return result;
		}

		//���̴� ���ҽ� �� ����
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		shaderResourceViewDesc.Format = image.GetMetadata().format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		//���̴� ���ҽ� �� ����
		result = pDevice->CreateShaderResourceView(m_resource, &shaderResourceViewDesc, &m_resourceView);
		if (FAILED(result))
		{
			m_resourceView = nullptr;

			return result;
		}

		return S_OK;
	}

	virtual bool OnClick(long& mouseX, long& mouseY)
	{
		bool result = false;

		return result;
	}

	virtual void SetScale(DirectX::XMFLOAT3 size)
	{
		m_vertices[0] = DirectX::XMVectorSet(-0.5f * size.x , 0.5f * size.y, 0.0f, 1.0f);//�»��
		m_vertices[1] = DirectX::XMVectorSet(0.5f * size.x, 0.5f * size.y, 0.0f, 1.0f);//����
		m_vertices[2] = DirectX::XMVectorSet(-0.5f * size.x, -0.5f * size.y, 0.0f, 1.0f);//���ϴ�
		m_vertices[3] = DirectX::XMVectorSet(0.5f * size.x, -0.5f * size.y, 0.0f, 1.0f);//���ϴ�
	}

	virtual void Render(ID3D11DeviceContext* pDeviceContext)
	{
		pDeviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &m_stride, &m_offset);//���۸� ��ǲ ������� ���ε�
		pDeviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);//���۸� ��ǲ ������� ���ε�
		pDeviceContext->PSSetShaderResources(0, 1, &m_resourceView);//���̴� ���ҽ� �並 �ȼ� ���̴��� ����
		pDeviceContext->DrawIndexed(m_indexCount, 0, 0);//�׸���
	}

private:
	ID3D11Buffer* m_vertexBuffer;//���� ����
	ID3D11Buffer* m_indexBuffer;//�ε��� ����
	ID3D11Resource* m_resource;//�ؽ��� ���ҽ�
	ID3D11ShaderResourceView* m_resourceView;//���̴� ���ҽ� ��

	DirectX::XMVECTOR* m_vertices;//���� �迭
	UINT* m_indices;//�ε��� �迭
	UINT m_indexCount;//�ε��� ����
	UINT m_stride;//�迭���� ���� ����Ʈ
	UINT m_offset;
};

#endif
