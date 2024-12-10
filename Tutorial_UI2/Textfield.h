#ifndef _TEXT_FIELD_
#define _TEXT_FIELD_

#include "RectTransform.h"
#include "RectTexture2D.h"
#include "ShaderManager.h"
#include "textclass.h"

class TextField : public RectTransform , public RectTexture2D
{
public:
	TextField()
	{
		m_text = L"";
		m_textFieldState = TextFieldState::IDLE;
	}

	HRESULT Initialize(ID3D11Device* pDevice, const wchar_t* idleTexturePath, const wchar_t* focusTexturePath)
	{
		HRESULT result;

		result = InitializeBuffer(pDevice);
		if (FAILED(result))
		{
			return result;
		}

		const wchar_t* filepath[2] = { idleTexturePath , focusTexturePath };

		result = SetTexture(pDevice, filepath, _countof(filepath));
		if (FAILED(result))
		{
			return result;
		}

		return S_OK;
	}

	void Frame(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj, const float& mouseX, const float& mouseY)
	{
		bool state;

		if (InputClass::GetInstance().GetLeftMouseButtonPressedAndReleased())
		{
			state = RayCast(GetWorldMatrix(), view, proj, mouseX, mouseY);

			if (state)
			{
				m_textFieldState = TextFieldState::FOCUS;
				InputClass::GetInstance().SetTextInputEnabled(true);

				return;
			}
			else
			{
				m_textFieldState = TextFieldState::IDLE;
				InputClass::GetInstance().SetTextInputEnabled(false);

				return;
			}
		}

		UpdateText();

		return;
	}

	bool Render(ID3D11DeviceContext* pDeviceContext, TextClass* pTextClass, UIShaderClass* pUIShader, const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj)
	{
		bool result;

		result = pUIShader->Render(pDeviceContext, world, view, proj);
		if (!result)
		{
			return false;
		}

		if (m_textFieldState != TextFieldState::FOCUS)
		{
			Draw(pDeviceContext, 0);//0번째 텍스쳐 지정해서 그리기
		}
		else
		{
			Draw(pDeviceContext, 1);//1번째 텍스쳐 지정해서 그리기
		}

		float x, y;

		GetScreenPos(x, y);

		x = 100.0f;
		y = 0.0f;
		////////////////////////////////위치 조정 안돼요 마저하셈
		pTextClass->RenderText(m_text.c_str(), x, y, 300, 300);

		return true;
	}

private:
	enum struct TextFieldState
	{
		IDLE,
		FOCUS
	};

	void UpdateText()
	{
		//입력 가능 상태일 때 InputClass의 텍스트를 가져옴
		if (m_textFieldState != TextFieldState::IDLE)
		{
			m_text = InputClass::GetInstance().GetText();

			return;
		}
	}

private:
	std::wstring m_text;
	TextFieldState m_textFieldState;
};

#endif
