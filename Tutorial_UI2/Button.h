#ifndef _BUTTON_
#define _BUTTON_

#include "RectTransform.h"
#include "RectTexture2D.h"
#include "ShaderManager.h"
#include "InputClass.h"
#include "EventClass.h"

class Button : public RectTransform, public RectTexture2D
{
public:
	Button()
	{
		m_btnState = ButtonState::BUTTON_IDLE;
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

		result = SetTexture(pDevice, filepath, _countof(filepath));
		if (FAILED(result))
		{
			return result;
		}

		return S_OK;
	}

	void Frame(DirectX::XMMATRIX view, DirectX::XMMATRIX proj, float mouseX, float mouseY)
	{
		bool state;

		if (m_skip)
		{
			if (InputClass::GetInstance().GetLeftMouseButtonReleased())//���콺 Ŭ���� Release �ϱ� ������ ��� �ǳʶٱ�
			{
				m_skip = false;//��ŵ ����
			}

			return;//�Լ� ���� ���� (�ڿ� �ִ� ������ ���ʿ��ϰ� ��)
		}

		//���콺�� ��ư ���� ���� ���� ��
		state = RayCast(GetWorldMatrix(), view, proj, mouseX, mouseY);
		if (!state)
		{
			m_btnState = ButtonState::BUTTON_IDLE;//�⺻ ���·� �ʱ�ȭ

			if (InputClass::GetInstance().GetLeftMouseButtonPressed())//���� �ۿ��� ���� ��� Release �ϱ� ������ ����ĳ��Ʈ ��ŵ
			{
				m_skip = true;//��ŵ ����
			}

			return;//�Լ� ���� ����
		}
		else//��ư ���� ���� ���� ��
		{
			if (m_btnState != ButtonState::BUTTON_PRESSED)//��ư ���°� Idle Ȥ�� Hover �� ���
			{
				if (InputClass::GetInstance().GetLeftMouseButtonPressed())//��ư Ŭ�� Ȯ�� �� ���̸� Pressed ���·� ��ȯ
				{
					m_btnState = ButtonState::BUTTON_PRESSED;
				}
				else//Ŭ���� ���� ���
				{
					m_btnState = ButtonState::BUTTON_HOVER;//��ư ���� Hover�� ��ȯ
				}
			}
			else//Pressed ������ ���
			{
				if (InputClass::GetInstance().GetLeftMouseButtonReleased())//���� ������ Release ���� �� ���� ��ȯ
				{
					m_btnState = ButtonState::BUTTON_RELEASED;
				}
			}
		}

		return;
	}

	bool IsPressed()
	{
		//��ư�� ���ȴٸ� ���� ��� ó�� ��ư�� ���ȴ��� üũ�� �� ���� Release ���¸� ����
		//���� ��ư�� �� �����Ӹ��� ���ȴ��� üũ�ؾ��� (���ܷ� ��ư ������ ��� Ŭ���� �ϰ� �Ǹ� �ʱ���·�)
		if (m_btnState == ButtonState::BUTTON_RELEASED)
		{
			m_btnState = ButtonState::BUTTON_IDLE;//��ư ���ȴ��� üũ�ߴٸ� �ʱ� ���·� �ǵ����� true ����

			return true;
		}

		return false;
	}

	void SetActive(bool state)
	{
		m_skip = state;
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

		switch (m_btnState)
		{
		case ButtonState::BUTTON_IDLE:
			Draw(pDeviceContext, 0);
			break;
		case ButtonState::BUTTON_HOVER:
			Draw(pDeviceContext, 1);
			break;
		case ButtonState::BUTTON_PRESSED:
			Draw(pDeviceContext, 2);
			break;
		case ButtonState::BUTTON_RELEASED:
			Draw(pDeviceContext, 0);
			break;
		}

		return true;
	}

private:
		enum struct ButtonState
		{
			BUTTON_IDLE,//���� ����
			BUTTON_HOVER,//���콺�� ��ư ���� �ø� ����
			BUTTON_PRESSED,//������ �ִ� ���� ����
			BUTTON_RELEASED//��ư�� ���ȴٰ� ������ ����
		};

private:
	ButtonState m_btnState;
	bool m_skip;
};

#endif
