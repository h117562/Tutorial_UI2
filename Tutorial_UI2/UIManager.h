#ifndef _INTERFACE_MANAGER_H_
#define _INTERFACE_MANAGER_H_

#include "DebugInfo.h"
#include "ShaderManager.h"
#include "InputClass.h"

class UIManager
{
public:
	UIManager();
	~UIManager();
	UIManager(const UIManager&);

	bool Initialize(D3DClass*);
	void Frame(D3DClass*, HWND hwnd, ShaderManager*, TextClass*, CameraClass*, InputClass*);
	void Shutdown();

private:
	DebugInfo* m_debugUI;

	bool m_enableDebugUI;
};

#endif