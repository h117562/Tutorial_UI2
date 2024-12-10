#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

#include <Windows.h>
#include <fstream>
#include <string>

#include "ApplicationClass.h"
#include "InputClass.h"
#include "FrameTimer.h"
#include "EventClass.h"

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
	LPCWSTR m_applicationName;	//���� ���α׷� �̸�
	HINSTANCE m_hinstance;		//�ν��Ͻ� �ڵ�
	HWND m_hwnd;				//������ �ڵ�

	ApplicationClass* m_applicationClass;
	FrameTimer* m_frameTimer;

};

static SystemClass* ApplicationHandle = nullptr;
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#endif