#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

#include <Windows.h>
#include <fstream>
#include <string>

#include "ApplicationClass.h"
#include "InputClass.h"
#include "FrameTimer.h"

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

using namespace std;

class SystemClass
{
public:
	SystemClass();
	~SystemClass();
	SystemClass(const SystemClass&);

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool InitializeWindow();
	void InitializeDisplaySettings();
	bool Frame();

	bool SearchOptions(string, const char*);
	bool StringToBool(string);
private:
	LPCWSTR m_applicationName;	//응용 프로그램 이름
	HINSTANCE m_hinstance;		//인스턴스 핸들
	HWND m_hwnd;				//윈도우 핸들

	ApplicationClass* m_applicationClass;
	InputClass* m_inputClass;
	FrameTimer* m_frameTimer;

};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static SystemClass* ApplicationHandle = nullptr;

#endif