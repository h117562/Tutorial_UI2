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

		if (!m_prev)//���� ���°� false ���� ��
		{
			//��ư ���� ������ ������ m_prev�� ���¸� true�� �ٲ�
			//���콺 ���� ��ư�� ������ ���
			result = pInputClass->GetLeftMouseButtonDown();
			if (result)
			{
				//����ĳ��Ʈ �Ͽ� ������� �浹 üũ
				result = RayCast(view, proj, mouseX, mouseY);
				if (result)
				{
					m_prev = true;

					return false;
				}
			}
		}
		else//���� ���°� true �� ��
		{
			//��ư ���� ������ ���콺 ��ư�� Release ���� �� ��ư�� �����ٰ� ����
			//���콺 ��ư Release Ȯ��
			result = pInputClass->GetLeftMouseButtonUp();
			if (result)
			{
				//����ĳ��Ʈ �Ͽ� ������� �浹 üũ
				result = RayCast(view, proj, mouseX, mouseY);
				if (result)
				{
					return true;
				}
				else
				{
					//��ư ���� �ۿ��� Release �ϸ� �ʱ���·� �ǵ���
					m_prev = false;
				}
			}
		}
		
		return false;
	}

	bool Render(ID3D11DeviceContext* pDeviceContext,UIShaderClass* pUIShader, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj)
	{
		bool result;

		//��� ������Ʈ
		result = pUIShader->Render(pDeviceContext ,world, view, proj);
		if (!result)
		{
			return false;
		}


		if (!m_prev)
		{
			//��ư �׸���
			result = Draw1(pDeviceContext);
			if (!result)
			{
				return false;
			}
		}
		else
		{
			//��ư �׸���
			result = Draw2(pDeviceContext);
			if (!result)
			{
				return false;
			}
		}

		return true;
	}

private:
	bool m_prev = false;
};

#endif
