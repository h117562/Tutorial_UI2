#ifndef _COLOR_SHADER_CLASS_H_
#define _COLOR_SHADER_CLASS_H_

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <string>

#include "GraphicStructures.h"

using namespace DirectX;

class ColorShaderClass
{
public:
	ColorShaderClass();
	~ColorShaderClass();
	ColorShaderClass(const ColorShaderClass&);

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);

private:
	void OutputShaderErrorMessage(ID3DBlob*, HWND);
	bool UpdateShaderBuffers(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;

	UINT m_bufferCount;
	UINT m_startNumber;
};

#endif