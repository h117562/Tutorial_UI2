#ifndef _BUTTON_
#define _BUTTON_

#include "RectTransform.h"
#include "RectTexture2D.h"
#include "ShaderManager.h"
#include "InputClass.h"

enum ButtonState
{
	BUTTON_IDLE,//���� ����
	BUTTON_HOVER,//���콺�� ��ư ���� �ø� ����
	BUTTON_PRESSED,//������ �ִ� ���� ����
	BUTTON_RELEASED//��ư�� ���ȴٰ� ������ ����
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
			if (pInputClass->GetLeftMouseButtonReleased())//���콺 Ŭ���� Release �ϱ� ������ ��� �ǳʶٱ�
			{
				m_skip = false;//��ŵ ����
			}

			return;//�Լ� ����
		}

		//���콺�� ��ư ���� ���� ���� ��
		result = RayCast(GetWorldMatrix(), view, proj, mouseX, mouseY);
		if (!result)
		{
			m_state = BUTTON_IDLE;//�⺻ ���·� �ʱ�ȭ

			if (pInputClass->GetLeftMouseButtonPressed())//���� �ۿ��� ���� ��� Release �ϱ� ������ ����ĳ��Ʈ ��ŵ
			{
				m_skip = true;//��ŵ ����
			}

			return;//�Լ� ����
		}
		else//��ư ���� ���� ���� ��
		{
			if (m_state != BUTTON_PRESSED)//��ư ���°� Idle Ȥ�� Hover �� ���
			{
				if (pInputClass->GetLeftMouseButtonPressed())//��ư Ŭ�� Ȯ�� �� ���̸� Pressed ���·� ��ȯ
				{
					m_state = BUTTON_PRESSED;
				}
				else//Ŭ���� ���� ���
				{
					m_state = BUTTON_HOVER;//��ư ���� Hover�� ��ȯ
				}
			}
			else//Pressed ������ ���
			{
				if (pInputClass->GetLeftMouseButtonReleased())//���� ������ Release ���� �� ���� ��ȯ
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

		//��� ������Ʈ
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
