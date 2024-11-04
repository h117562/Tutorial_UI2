#ifndef _APPLICATION_CLASS_H_
#define _APPLICATION_CLASS_H_

#include "D3DClass.h"
#include "ShaderManager.h"
#include "CameraClass.h"
#include "InputClass.h"
#include "FrameTimer.h"
#include "UIManager.h"
#include "TextClass.h"

#include "RectangleModel.h"
#include "MirrorModel.h"

class ApplicationClass
{
public:
	ApplicationClass();
	~ApplicationClass();
	ApplicationClass(const ApplicationClass&);

	bool Initialize(HWND);
	void Shutdown();
	bool Frame(HWND, InputClass*, FrameTimer*);

private:
	void HandleInput(InputClass*, FrameTimer*);
	void Render(HWND, InputClass*);

private:
	D3DClass* m_Direct3D;
	TextClass* m_TextClass;
	ShaderManager* m_ShaderManager;
	CameraClass* m_CameraClass;
	UIManager* m_uiManager;

	RectangleModel* m_rectangle;
	MirrorModel* m_mirror;
};
#endif