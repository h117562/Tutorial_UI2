#ifndef _RECT_TEXTURE_2D_
#define _RECT_TEXTURE_2D_

#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "../data/DirectXTex.lib")

#include "../data/DirectXTex.h"

class RectTexture2D
{
public:
	RectTexture2D()
	{
		m_vertices = new DirectX::XMFLOAT3[4];

		m_vertices[0] = DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f);
		m_vertices[1] = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);
		m_vertices[2] = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f);
		m_vertices[3] = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f);

		m_indices = new unsigned int[6];

		m_indices[0] = 0;
		m_indices[1] = 3;
		m_indices[2] = 2;
		m_indices[3] = 3;
		m_indices[4] = 0;
		m_indices[5] = 1;
	}

	~RectTexture2D()
	{
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

	virtual void SetTexture() = 0;
	virtual void Render() = 0;
	virtual void RayCast() = 0;
	virtual void SetScale() = 0;

private:
	DirectX::XMFLOAT3* m_vertices;
	unsigned int* m_indices;

};

#endif
