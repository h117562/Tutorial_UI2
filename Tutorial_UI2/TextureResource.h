#ifndef _TEXTURE_RESOURCE_
#define _TEXTURE_RESOURCE_

#pragma comment(lib, "../data/DirectXTex.lib")

#include "../data/DirectXTex.h"

class TextureResource
{
public:
	TextureResource()
	{
		resource = nullptr;
		resourceView = nullptr;
	}

	~TextureResource()
	{
		if (resource)
		{
			resource->Release();
			resource = nullptr;
		}

		if (resourceView)
		{
			resourceView->Release();
			resourceView = nullptr;
		}
	}

	ID3D11Resource* GetResource()
	{
		return resource;
	};

	ID3D11ShaderResourceView* GetResourceView()
	{
		return resourceView;
	};

	HRESULT InitializeResourceView(ID3D11Device* pDevice, const wchar_t* filePath)
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
			&resource);

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
		shaderResourceViewDesc.Texture2D.MipLevels = -1;

		//���̴� ���ҽ� �� ����
		result = pDevice->CreateShaderResourceView(resource, &shaderResourceViewDesc, &resourceView);
		if (FAILED(result))
		{
			return result;
		}

		return S_OK;
	}

private:
	ID3D11Resource* resource;
	ID3D11ShaderResourceView* resourceView;
};

#endif