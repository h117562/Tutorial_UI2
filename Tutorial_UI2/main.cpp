
#include <Windows.h>

#include "Global.h"
#include "Systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	bool result;

	SystemClass* systemClass;

	//SystemClass 생성
	systemClass = new SystemClass;
	if (!systemClass)
	{
		return 0;
	}

	//SystemClass 초기화
	result = systemClass->Initialize();
	if (result)
	{
		systemClass->Run();
	}

	//System Class의 모든 자원 반환
	systemClass->Shutdown();
	delete systemClass;
	systemClass = 0;

	return 0;
}