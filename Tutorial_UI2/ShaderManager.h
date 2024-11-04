#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

#include "TextureShaderClass.h"
#include "ColorShaderClass.h"

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();
	ShaderManager(const ShaderManager&);

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	TextureShaderClass* GetTextureShader();
	ColorShaderClass* GetColorShader();

private:
	TextureShaderClass* m_TextureShader;
	ColorShaderClass* m_ColorShader;

};

#endif