#ifndef _BUTTON_
#define _BUTTON_

#include "RectTransform.h"
#include "RectTexture2D.h"
#include "ShaderManager.h"
#include "InputClass.h"

enum ButtonState
{
	BUTTON_IDLE,//평상시 상태
	BUTTON_HOVER,//마우스를 버튼 위에 올렸을 때
	BUTTON_PRESSED,//눌린 상태
	BUTTON_RELEASED//눌린 상태
};

class Button : public RectTransform, public RectTexture2D
{
public:
	Button()
	{
		m_state = BUTTON_IDLE;
		m_prev = false;
	}

	void Frame(InputClass* pInputClass, DirectX::XMMATRIX view, DirectX::XMMATRIX proj, float mouseX, float mouseY)
	{
		bool result;

		result = RayCast(GetWorldMatrix(), view, proj, mouseX, mouseY);
		if (!result)
		{
			m_state = BUTTON_IDLE;
			m_prev = false;
		}
		else
		{
			if (!m_prev)
			{
				if (pInputClass->GetLeftMouseButtonPressed())
				{
					m_prev = true;

					m_state = BUTTON_PRESSED;
				}
				else
				{
					m_state = BUTTON_HOVER;
				}
			}
			else
			{
				if (pInputClass->GetLeftMouseButtonReleased())
				{
					m_prev = false;

					m_state = BUTTON_RELEASED;
				}
			}
		}

		return;
	}

	bool IsPressed()
	{
		if (m_state == BUTTON_RELEASED)
		{
			m_state = 0;

			return true;
		}

		return false;
	}

	bool Render(ID3D11DeviceContext* pDeviceContext,UIShaderClass* pUIShader,const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj)
	{
		bool result;

		//행렬 업데이트
		result = pUIShader->Render(pDeviceContext ,world, view, proj);
		if (!result)
		{
			return false;
		}

		switch (m_state)
		{
		case BUTTON_IDLE:
			Draw(pDeviceContext, 0);
			break;
		case BUTTON_HOVER:
			Draw(pDeviceContext, 1);
			break;
		case BUTTON_PRESSED:
			Draw(pDeviceContext, 0);
			break;
		}

		return true;
	}

private:
	UINT m_state;
	bool m_prev;
};

#endif
