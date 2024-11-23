#ifndef _BUTTON_
#define _BUTTON_

#include "RectTransform.h"
#include "RectTexture2D.h"
#include "ShaderManager.h"
#include "InputClass.h"

class Button : public RectTransform, public RectTexture2D
{
public:

	bool IsPressed(InputClass* pInputClass, DirectX::XMMATRIX view, DirectX::XMMATRIX proj, float mouseX, float mouseY)
	{
		bool result;

		if (!m_prev)//이전 상태가 false 였을 때
		{
			//버튼 범위 내에서 누르면 m_prev의 상태를 true로 바꿈
			//마우스 왼쪽 버튼을 눌렀을 경우
			result = pInputClass->GetLeftMouseButtonDown();
			if (result)
			{
				//레이캐스트 하여 폴리곤과 충돌 체크
				result = RayCast(view, proj, mouseX, mouseY);
				if (result)
				{
					m_prev = true;

					return false;
				}
			}
		}
		else//이전 상태가 true 일 때
		{
			//버튼 범위 내에서 마우스 버튼을 Release 했을 때 버튼을 눌렀다고 판정
			//마우스 버튼 Release 확인
			result = pInputClass->GetLeftMouseButtonUp();
			if (result)
			{
				//레이캐스트 하여 폴리곤과 충돌 체크
				result = RayCast(view, proj, mouseX, mouseY);
				if (result)
				{
					return true;
				}
				else
				{
					//버튼 범위 밖에서 Release 하면 초기상태로 되돌림
					m_prev = false;
				}
			}
		}
		
		return false;
	}

	bool Render(ID3D11DeviceContext* pDeviceContext,UIShaderClass* pUIShader, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj)
	{
		bool result;

		//행렬 업데이트
		result = pUIShader->Render(pDeviceContext ,world, view, proj);
		if (!result)
		{
			return false;
		}

		if (!m_prev)
		{
			//버튼 그리기
			Draw(pDeviceContext, 0);
		}
		else
		{
			//버튼 그리기
			Draw(pDeviceContext, 1);
		}

		return true;
	}

private:
	bool m_prev = false;
};

#endif
