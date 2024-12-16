#ifndef _RECT_TEXTURE_2D_
#define _RECT_TEXTURE_2D_

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>

#include "TextureResource.h"
#include "GraphicStructures.h"

class RectTexture2D
{
protected://��� ���� Ŭ���������� ��밡���ϰ�
	RectTexture2D()
	{
		m_vertexBuffer = nullptr;
		m_indexBuffer = nullptr;
		m_indexCount = 6;
		m_stride = sizeof(VertexUV);
		m_offset = 0;

		m_textures = nullptr;
		m_textureCount = 0;

		//��ȯ���� �ٷ� ����ĳ��Ʈ�� ����ϱ� ���ؼ� XMVECTOR�� ����
		m_vertices = new VertexUV[4];

		m_vertices[0].position = DirectX::XMVectorSet(-0.5f, 0.5f, 0.0f, 1.0f);//�»��
		m_vertices[1].position = DirectX::XMVectorSet(0.5f, 0.5f, 0.0f, 1.0f);//����
		m_vertices[2].position = DirectX::XMVectorSet(-0.5f, -0.5f, 0.0f, 1.0f);//���ϴ�
		m_vertices[3].position = DirectX::XMVectorSet(0.5f, -0.5f, 0.0f, 1.0f);//���ϴ�

		m_vertices[0].textureCoord = DirectX::XMFLOAT2(0.0f, 0.0f);//�»��
		m_vertices[1].textureCoord = DirectX::XMFLOAT2(1.0f, 0.0f);//����
		m_vertices[2].textureCoord = DirectX::XMFLOAT2(0.0f, 1.0f);//���ϴ�
		m_vertices[3].textureCoord = DirectX::XMFLOAT2(1.0f, 1.0f);//���ϴ�

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

		if (m_textures)
		{
			delete[] m_textures;
			m_textures = nullptr;
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

	HRESULT InitializeBuffer(ID3D11Device* pDevice)
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

	HRESULT SetTexture(ID3D11Device* pDevice, const wchar_t* filePath[], const unsigned int& count)
	{
		HRESULT result;

		m_textures = new TextureResource[count];
		m_textureCount = count;

		for (UINT i = 0; i < count; i++)
		{
			result = m_textures[i].InitializeResourceView(pDevice, filePath[i]);
			if (FAILED(result))
			{
				return result;
			}
		}

		return result;
	}

	void Draw(ID3D11DeviceContext* pDeviceContext, const unsigned int& index)
	{
		ID3D11ShaderResourceView* srv = nullptr;

		if (m_textureCount >= index)
		{
			srv = m_textures[index].GetResourceView();
		}

		pDeviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &m_stride, &m_offset);//���۸� ��ǲ ������� ���ε�
		pDeviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);//���۸� ��ǲ ������� ���ε�
		pDeviceContext->PSSetShaderResources(0, 1, &srv);//���̴� ���ҽ� �並 �ȼ� ���̴��� ����
		pDeviceContext->DrawIndexed(m_indexCount, 0, 0);//�׸���
	}

	bool RayCast(
		const DirectX::XMMATRIX& world,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const float& mouseX,
		const float& mouseY)
	{
		bool result = false;
		float dist = 0.0f;//�浹 �Ÿ�
		DirectX::XMVECTOR origin = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		DirectX::XMVECTOR direction = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);//orthoMatrix�� ��� �׳� Z �������� �ص� ��

		//���� ����
		GetRay(world, view, projection, mouseX, mouseY, origin);

		//������� �浹üũ
		result = DirectX::TriangleTests::Intersects(origin, direction, 
			m_vertices[m_indices[0]].position,
			m_vertices[m_indices[1]].position,
			m_vertices[m_indices[2]].position, dist);
		if (result)
		{
			return true;
		}

		//������� �浹üũ
		result = DirectX::TriangleTests::Intersects(origin, direction, 
			m_vertices[m_indices[3]].position, 
			m_vertices[m_indices[4]].position, 
			m_vertices[m_indices[5]].position, dist);
		if (result)
		{
			return true;
		}

		return false;
	}

private:
	void GetRay(
		const DirectX::XMMATRIX& w,
		const DirectX::XMMATRIX& v,
		const DirectX::XMMATRIX& p,
		const float& mouseX,
		const float& mouseY, 
		DirectX::XMVECTOR& origin)
	{
		DirectX::XMMATRIX inverseWorld, inverseViewProjection;

		inverseViewProjection = DirectX::XMMatrixMultiply(v, p);//��, ���� ��� ��
		inverseViewProjection = DirectX::XMMatrixInverse(nullptr, inverseViewProjection);//�����

		origin = DirectX::XMVectorSet(mouseX, mouseY, 0.0f, 1.0f);//���� ����
		origin = DirectX::XMVector3Transform(origin, inverseViewProjection);//���� ��ȯ

		inverseWorld = DirectX::XMMatrixInverse(nullptr, w);//�����

		origin = XMVector3TransformCoord(origin, inverseWorld);//���� ��ȯ

		//direction = DirectX::XMVectorSet(mouseX, mouseY, 1.0f, 1.0f);//���� ��
		//direction = DirectX::XMVector3Transform(direction, inverseViewProjection);//��.���� ����ķ� ���� ��ȯ
		//direction = DirectX::XMVectorSubtract(direction, origin);//���� ������ ������ ���� ������ ����
		//direction = DirectX::XMVector3Normalize(direction);//���� ������ ����ȭ
	}

private:
	ID3D11Buffer* m_vertexBuffer;//���� ����
	ID3D11Buffer* m_indexBuffer;//�ε��� ����

	TextureResource* m_textures;//�ؽ��� ���ҽ�
	UINT m_textureCount;//�ؽ��� ����

	VertexUV* m_vertices;//���� �迭
	UINT* m_indices;//�ε��� �迭
	UINT m_indexCount;//�ε��� ����
	UINT m_stride;//�迭���� ���� ����Ʈ
	UINT m_offset;
};

#endif
