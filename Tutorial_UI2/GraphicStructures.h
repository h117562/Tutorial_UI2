#ifndef _GRAPHIC_STRUCTURES_H_
#define _GRAPHIC_STRUCTURES_H_

#include <directxmath.h>

struct VertexUV
{
	DirectX::XMVECTOR position;
	DirectX::XMFLOAT2 textureCoord;
};

struct MatrixBuffer
{
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projectionMatrix;
};

struct TextureResource
{
public:
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

private:
	ID3D11Resource* resource;
	ID3D11ShaderResourceView* resourceView;
};

#endif