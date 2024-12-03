#ifndef _TEST_CANVAS_
#define _TEST_CANVAS_

#include "D3DClass.h"
#include "TextClass.h"
#include "InputClass.h"
#include "ShaderManager.h"
#include "CameraClass.h"

//Elements
#include "Button.h"
#include "Plane.h"

extern unsigned int SCREEN_WIDTH;	//윈도우 너비
extern unsigned int SCREEN_HEIGHT;	//윈도우 높이

class TestCanvas
{
public:
	TestCanvas();
	~TestCanvas();

	bool Initialize(ID3D11Device*);
	bool Frame(D3DClass*, HWND, ShaderManager*, TextClass*, CameraClass*, InputClass*);
	bool Render(D3DClass*, ShaderManager*, const XMMATRIX&, const XMMATRIX&);
	void SetActive();
	bool GetActive();

private:
	Button* m_btn;
	Plane* m_plane;

	bool m_active;
};

#endif