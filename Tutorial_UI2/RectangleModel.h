#ifndef _RECTANGLE_MODEL_H_
#define _RECTANGLE_MODEL_H_

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "../data/DirectXTex.lib")

#include "../data/DirectXTex.h"

#include <d3d11.h>

#include "GraphicStructures.h"

class RectangleModel
{
public:
	RectangleModel();
	~RectangleModel();
	RectangleModel(const RectangleModel&);

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

private:
	VertexTextureCoord* m_vertices;
	UINT* m_indices;
	UINT m_indexCount;

	ID3D11Resource* m_resource;
	ID3D11ShaderResourceView* m_resourceView;

	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;

	UINT stride = sizeof(VertexTextureCoord);
	UINT offset = 0;

};

#endif