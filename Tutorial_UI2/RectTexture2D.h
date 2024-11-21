#ifndef _RECT_TEXTURE_2D_
#define _RECT_TEXTURE_2D_

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "../data/DirectXTex.lib")

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>

#include "../data/DirectXTex.h"
#include "GraphicStructures.h"

class RectTexture2D
{
public:
	virtual HRESULT InitializeBuffer(ID3D11Device* pDevice)
	{
		HRESULT result;

		//정점 버퍼 설정
		D3D11_BUFFER_DESC vertexBufferDesc;
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VertexUV) * m_indexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = m_vertices;

		//정점 버퍼 생성
		result = pDevice->CreateBuffer(&vertexBufferDesc, &initData, &m_vertexBuffer);
		if (FAILED(result))
		{
			return result;
		}

		//인덱스 버퍼 설정
		D3D11_BUFFER_DESC indexBufferDesc;
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(UINT) * m_indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;
		initData.pSysMem = &m_indices[0];


		//인덱스 버퍼 생성
		result = pDevice->CreateBuffer(&indexBufferDesc, &initData, &m_indexBuffer);
		if (FAILED(result))
		{
			return result;
		}

		return S_OK;
	}

	virtual HRESULT SetTexture1(ID3D11Device* pDevice, const wchar_t* filePath)
	{
		HRESULT result;

		result = IntializeResource(pDevice, filePath, true);

		return result;
	}

	virtual HRESULT SetTexture2(ID3D11Device* pDevice, const wchar_t* filePath)
	{
		HRESULT result;

		result = IntializeResource(pDevice, filePath, false);

		return result;
	}

	virtual void SetScale(DirectX::XMFLOAT3 size)
	{
		m_vertices[0].position = DirectX::XMVectorSet(-0.5f * size.x, 0.5f * size.y, 0.0f, 1.0f);//좌상단
		m_vertices[1].position = DirectX::XMVectorSet(0.5f * size.x, 0.5f * size.y, 0.0f, 1.0f);//우상단
		m_vertices[2].position = DirectX::XMVectorSet(-0.5f * size.x, -0.5f * size.y, 0.0f, 1.0f);//좌하단
		m_vertices[3].position = DirectX::XMVectorSet(0.5f * size.x, -0.5f * size.y, 0.0f, 1.0f);//우하단
	}

protected://상속 받은 클래스에서만 사용가능하게
	RectTexture2D()
	{
		 m_vertexBuffer = nullptr;
		 m_indexBuffer = nullptr;
		 m_resource1 = nullptr;
		 m_resource2 = nullptr;
		 m_resourceView1 = nullptr;
		 m_resourceView2 = nullptr;
		 m_indexCount = 6;
		 m_stride = sizeof(VertexUV);
		 m_offset = 0;

		//변환없이 바로 레이캐스트에 사용하기 위해서 XMVECTOR로 선언
		m_vertices = new VertexUV[4];

		m_vertices[0].position = DirectX::XMVectorSet(-0.5f, 0.5f, 0.0f, 1.0f);//좌상단
		m_vertices[1].position = DirectX::XMVectorSet(0.5f, 0.5f, 0.0f, 1.0f);//우상단
		m_vertices[2].position = DirectX::XMVectorSet(-0.5f, -0.5f, 0.0f, 1.0f);//좌하단
		m_vertices[3].position = DirectX::XMVectorSet(0.5f, -0.5f, 0.0f, 1.0f);//우하단

		m_vertices[0].textureCoord = DirectX::XMFLOAT2(0.0f, 0.0f);//좌상단
		m_vertices[1].textureCoord = DirectX::XMFLOAT2(1.0f, 0.0f);//우상단
		m_vertices[2].textureCoord = DirectX::XMFLOAT2(0.0f, 1.0f);//좌하단
		m_vertices[3].textureCoord = DirectX::XMFLOAT2(1.0f, 1.0f);//우하단

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

		if (m_resource1)
		{
			m_resource1->Release();
			m_resource1 = nullptr;
		}

		if (m_resourceView1)
		{
			m_resourceView1->Release();
			m_resourceView1 = nullptr;
		}

		if (m_resource2)
		{
			m_resource2->Release();
			m_resource2 = nullptr;
		}

		if (m_resourceView2)
		{
			m_resourceView2->Release();
			m_resourceView2 = nullptr;
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

	virtual bool Draw1(ID3D11DeviceContext* pDeviceContext)
	{
		if (m_vertexBuffer == nullptr)
		{
			return false;
		}

		pDeviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &m_stride, &m_offset);//버퍼를 인풋 어셈블러에 바인딩
		pDeviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);//버퍼를 인풋 어셈블러에 바인딩
		pDeviceContext->PSSetShaderResources(0, 1, &m_resourceView1);//쉐이더 리소스 뷰를 픽셀 쉐이더에 연계
		pDeviceContext->DrawIndexed(m_indexCount, 0, 0);//그리기
	}

	virtual bool Draw2(ID3D11DeviceContext* pDeviceContext)
	{
		if (m_vertexBuffer == nullptr)
		{
			return false;
		}

		if (m_resourceView2 == nullptr)
		{
			Draw1(pDeviceContext);

			return true;
		}

		pDeviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &m_stride, &m_offset);//버퍼를 인풋 어셈블러에 바인딩
		pDeviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);//버퍼를 인풋 어셈블러에 바인딩
		pDeviceContext->PSSetShaderResources(0, 1, &m_resourceView2);//쉐이더 리소스 뷰를 픽셀 쉐이더에 연계
		pDeviceContext->DrawIndexed(m_indexCount, 0, 0);//그리기
	}

	virtual bool RayCast(
		const DirectX::XMMATRIX view,
		const DirectX::XMMATRIX projection,
		const float mouseX,
		const float mouseY) final
	{
		bool result = false;
		float dist = 0.0f;//충돌 거리
		DirectX::XMVECTOR origin = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		DirectX::XMVECTOR direction = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		//레이 생성
		GetRay(view, projection, mouseX, mouseY, origin, direction);

		//폴리곤과 충돌체크
		result = DirectX::TriangleTests::Intersects(origin, direction, 
			m_vertices[m_indices[0]].position,
			m_vertices[m_indices[1]].position,
			m_vertices[m_indices[2]].position, dist);
		if (result)
		{
			return true;
		}

		//폴리곤과 충돌체크
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
	virtual HRESULT IntializeResource(ID3D11Device* pDevice, const wchar_t* filePath, bool state)
	{
		HRESULT result;

		DirectX::ScratchImage image;

		//이미지 데이터를 로드
		result = DirectX::LoadFromWICFile(filePath, DirectX::WIC_FLAGS_NONE, nullptr, image);
		if (FAILED(result))
		{
			return result;
		}

		//원본 이미지의 데이터로 텍스쳐 리소스 생성
		result = DirectX::CreateTextureEx(pDevice,
			image.GetImages(),
			image.GetImageCount(),
			image.GetMetadata(),
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
			0,
			0,
			DirectX::CREATETEX_DEFAULT,
			state ? &m_resource1 : &m_resource2);

		if (FAILED(result))
		{
			return result;
		}

		//쉐이더 리소스 뷰 설정
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		shaderResourceViewDesc.Format = image.GetMetadata().format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		//쉐이더 리소스 뷰 생성
		result = pDevice->CreateShaderResourceView(state ? m_resource1 : m_resource2, &shaderResourceViewDesc, state ? &m_resourceView1 : &m_resourceView2);
		if (FAILED(result))
		{
			return result;
		}

		return S_OK;
	}

	virtual void GetRay(
		const DirectX::XMMATRIX v,
		const DirectX::XMMATRIX p,
		const float mouseX,
		const float mouseY, 
		DirectX::XMVECTOR& origin, 
		DirectX::XMVECTOR& direction)
	{
		DirectX::XMMATRIX inverseViewProjection;

		inverseViewProjection = DirectX::XMMatrixMultiply(v, p);//뷰, 투영 행렬 곱
		inverseViewProjection = DirectX::XMMatrixInverse(nullptr, inverseViewProjection);//역행렬

		origin = DirectX::XMVectorSet(mouseX, mouseY, 0.0f, 1.0f);//레이 시작
		origin = DirectX::XMVector3Transform(origin, inverseViewProjection);//벡터 변환

		direction = DirectX::XMVectorSet(mouseX, mouseY, 1.0f, 1.0f);//레이 끝
		direction = DirectX::XMVector3Transform(direction, inverseViewProjection);//뷰.투영 역행렬로 벡터 변환
		direction = DirectX::XMVectorSubtract(direction, origin);//레이 끝에서 시작을 빼서 방향을 구함
		direction = DirectX::XMVector3Normalize(direction);//레이 방향을 정규화
	}

private:
	ID3D11Buffer* m_vertexBuffer;//정점 버퍼
	ID3D11Buffer* m_indexBuffer;//인덱스 버퍼
	ID3D11Resource* m_resource1;//텍스쳐 리소스
	ID3D11Resource* m_resource2;//텍스쳐 리소스
	ID3D11ShaderResourceView* m_resourceView1;//쉐이더 리소스 뷰
	ID3D11ShaderResourceView* m_resourceView2;//쉐이더 리소스 뷰

	VertexUV* m_vertices;//정점 배열
	UINT* m_indices;//인덱스 배열
	UINT m_indexCount;//인덱스 개수
	UINT m_stride;//배열마다 간격 바이트
	UINT m_offset;
};

#endif
