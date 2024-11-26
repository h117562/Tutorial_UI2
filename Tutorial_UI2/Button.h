#ifndef _BUTTON_
#define _BUTTON_

#include "RectTransform.h"
#include "RectTexture2D.h"
#include "ShaderManager.h"
#include "InputClass.h"

enum ButtonState
{
	BUTTON_IDLE,//평상시 상태
	BUTTON_HOVER,//마우스를 버튼 위에 올린 상태
	BUTTON_PRESSED,//누르고 있는 중인 상태
	BUTTON_RELEASED//버튼이 눌렸다고 판정한 상태
};

class Button : public RectTransform, public RectTexture2D
{
public:
	Button()
	{
		m_state = BUTTON_IDLE;
		m_skip = false;
	}

	HRESULT Initialize(ID3D11Device* pDevice, const wchar_t* idleTexturePath, const wchar_t* hoverTexturePath, const wchar_t* pressedTexturePath)
	{
		HRESULT result;

		result = InitializeBuffer(pDevice);
		if (FAILED(result))
		{
			return result;
		}

		const wchar_t* filepath[3] = { idleTexturePath , hoverTexturePath , pressedTexturePath };

		result = SetTexture(pDevice, filepath, 3);
		if (FAILED(result))
		{
			return result;
		}

		return S_OK;
	}

	void Frame(InputClass* pInputClass, DirectX::XMMATRIX view, DirectX::XMMATRIX proj, float mouseX, float mouseY)
	{
		bool result;

		if (m_skip)
		{
			if (pInputClass->GetLeftMouseButtonReleased())//마우스 클릭을 Release 하기 전까진 계속 건너뛰기
			{
				m_skip = false;//스킵 해제
			}

			return;//함수 종료
		}

		//마우스가 버튼 범위 내에 없을 때
		result = RayCast(GetWorldMatrix(), view, proj, mouseX, mouseY);
		if (!result)
		{
			m_state = BUTTON_IDLE;//기본 상태로 초기화

			if (pInputClass->GetLeftMouseButtonPressed())//범위 밖에서 눌린 경우 Release 하기 전까진 레이캐스트 스킵
			{
				m_skip = true;//스킵 상태
			}

			return;//함수 종료
		}
		else//버튼 범위 내에 있을 때
		{
			if (m_state != BUTTON_PRESSED)//버튼 상태가 Idle 혹은 Hover 일 경우
			{
				if (pInputClass->GetLeftMouseButtonPressed())//버튼 클릭 확인 후 참이면 Pressed 상태로 전환
				{
					m_state = BUTTON_PRESSED;
				}
				else//클릭이 없을 경우
				{
					m_state = BUTTON_HOVER;//버튼 상태 Hover로 전환
				}
			}
			else//Pressed 상태일 경우
			{
				if (pInputClass->GetLeftMouseButtonReleased())//범위 내에서 Release 했을 때 상태 전환
				{
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
			Draw(pDeviceContext, 2);
			break;
		case BUTTON_RELEASED:
			Draw(pDeviceContext, 0);
			break;
		}

		return true;
	}

private:
	UINT m_state;
	bool m_skip;
};

#endif
