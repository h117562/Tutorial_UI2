#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d11.h>
#include <directxmath.h>

//////////////////////////////////////////////////////
//////					GLOBAL					//////
//////////////////////////////////////////////////////
extern bool FULL_SCREEN;			//전체화면 여부
extern bool VSYNC_ENABLED;			//VSync 여부
extern unsigned int SCREEN_WIDTH;	//윈도우 너비
extern unsigned int SCREEN_HEIGHT;	//윈도우 높이
extern float SCREEN_NEAR;			//가까운 Z좌표 
extern float SCREEN_FAR;			//먼 Z좌표
//////////////////////////////////////////////////////

class D3DClass
{
public:
	D3DClass();
	~D3DClass();
	D3DClass(const D3DClass&);

	bool Initialize(HWND);
	void Shutdown();

	void BeginScene(float r, float g, float b, float a);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	IDXGISwapChain* GetSwapChain();

	void GetProjectionMatrix(DirectX::XMMATRIX& projectionMatrix);
	void GetWorldMatrix(DirectX::XMMATRIX& worldMatrix);
	void GetOrthoMatrix(DirectX::XMMATRIX& orthoMatrix);

	void GetVideoCardInfo(char* description, int& memory);

	void TurnZBufferOn();
	void TurnZBufferOff();

	void EnableAlphaBlending();
	void DisableAlphaBlending();

	void EnableWireframe();
	void DisableWireframe();

private:
	int m_videoCardMemory;
	char m_videoCardDescription[128];

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	IDXGISwapChain* m_swapChain;
	ID3D11RenderTargetView* m_renderTargetView;
	D3D11_VIEWPORT m_viewport;

	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilState* m_depthDisabledStencilState;
	ID3D11DepthStencilState* m_mirrorState;
	ID3D11DepthStencilState* m_maskState;

	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11Texture2D* m_depthStencilBuffer;

	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;

	ID3D11RasterizerState* m_rasterState;
	ID3D11RasterizerState* m_rasterStateWireframe;

	DirectX::XMMATRIX m_worldMatrix;
	DirectX::XMMATRIX m_projectionMatrix;
	DirectX::XMMATRIX m_orthoMatrix;
};

#endif