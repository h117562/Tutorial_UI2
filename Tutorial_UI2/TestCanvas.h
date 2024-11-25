#ifndef _TEST_CANVAS_
#define _TEST_CANVAS_

#include "Button.h"
#include "D3DClass.h"
#include "TextClass.h"
#include "InputClass.h"
#include "ShaderManager.h"
#include "CameraClass.h"

extern unsigned int SCREEN_WIDTH;	//������ �ʺ�
extern unsigned int SCREEN_HEIGHT;	//������ ����

class TestCanvas
{
public:
	TestCanvas();
	~TestCanvas();

	bool Initialize(ID3D11Device*);
	bool Frame(D3DClass*, HWND, ShaderManager*, TextClass*, CameraClass*, InputClass*);
	bool Render(D3DClass*, ShaderManager*, const XMMATRIX&, const XMMATRIX&);
	void SetActive(bool state);
	bool GetActive();

private:
	Button* m_btn;

	bool m_active;
};

#endif