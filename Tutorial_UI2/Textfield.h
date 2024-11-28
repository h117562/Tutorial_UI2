#ifndef _TEXT_FIELD_
#define _TEXT_FIELD_

#include "RectTransform.h"
#include "RectTexture2D.h"
#include "ShaderManager.h"
#include "textclass.h"

class Textfield : public RectTransform , public RectTexture2D
{
public:
	Textfield()
	{
		m_inputEnable = false;
	}

	bool Initialize(ID3D11Device* pDevice, const wchar_t* idleTexturePath, const wchar_t* inputTexturePath)
	{

	}

	bool Frame()
	{

	}

	bool Render(ID3D11DeviceContext* pDeviceContext, UIShaderClass* pUIShader, const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj)
	{
		bool result;

		result = pUIShader->Render(pDeviceContext, world, view, proj);
		if (!result)
		{
			return false;
		}


		if (!m_inputEnable)
		{
			Draw(pDeviceContext, 0);//0번째 텍스쳐 지정해서 그리기
		}
		else
		{
			Draw(pDeviceContext, 1);//1번째 텍스쳐 지정해서 그리기
		}

		return true;
	}

private:
	bool m_inputEnable;
};

#endif
