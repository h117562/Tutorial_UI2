#ifndef _RECT_TRANSFORM_
#define _RECT_TRANSFORM_

#include <DirectXMath.h>
#include <d2d1.h>

#include "Global.h"

enum ALIGNMENT//���� �÷���
{
	ALIGNMENT_CENTER,//���߾�
	ALIGNMENT_LEFT,//���� �߾�
	ALIGNMENT_RIGHT,//���� �߾�
	ALIGNMENT_TOP,//��� �߾�
	ALIGNMENT_BOTTOM,//�ϴ� �߾�
	ALIGNMENT_LEFT_TOP,//�»��
	ALIGNMENT_LEFT_BOTTOM,//���ϴ�
	ALIGNMENT_RIGHT_TOP,//����
	ALIGNMENT_RIGHT_BOTTOM,//���ϴ�
};

class RectTransform//����� ��ġ ���� �� ���� ����� �����ϴ� �߻� Ŭ����
{
protected:
	RectTransform()
	{
		//�⺻ ���� ������
		m_localPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_worldPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_scale = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_align = ALIGNMENT_CENTER;
		m_worldMatrix = DirectX::XMMatrixIdentity();
		m_screenArea = D2D1::RectF(0.0f, 0.0f, 100.0f, 100.0f);
	}

	virtual ~RectTransform()
	{
	}

public:
	//��� ��ġ ����
	virtual void SetLocalPosition(const float& x, const float& y, const float& z)
	{
		m_localPosition = DirectX::XMFLOAT3(x, y, z);
	}

	//ȸ�� ����
	virtual void SetRotation(const float& x, const float& y, const float& z)
	{
		m_rotation = DirectX::XMFLOAT3(x, y, z);
	}

	//ũ�� ����
	virtual void SetScale(const float& x, const float& y, const float& z)
	{
		m_scale = DirectX::XMFLOAT3(x, y, z);
	}

	//���� ����
	virtual void SetAlign(const unsigned int& flag)
	{
		m_align = flag;
	}

	//��ü�� ��ġ, ȸ�� ���� �ݿ��� ���� ��ǥ��, ��ũ�� ��ǥ�� ������ �ֽ�ȭ
	virtual void UpdateTransform()
	{
		UpdateWorldMatrix();
		UpdateScreenArea();
	}

	//���� ��� ��������
	virtual DirectX::XMMATRIX GetWorldMatrix()
	{
		return m_worldMatrix;
	}

	//��� ��ǥ ���
	virtual DirectX::XMFLOAT3 GetLocalPosition()
	{
		return m_localPosition;
	}

	//���� ��ǥ ���
	virtual DirectX::XMFLOAT3 GetWorldPosition()
	{
		return m_worldPosition;
	}

	//ȸ�� ���
	virtual DirectX::XMFLOAT3 GetRotation()
	{
		return m_rotation;
	}

	//ũ�� ���
	virtual DirectX::XMFLOAT3 GetScale()
	{
		return m_scale;
	}

	//���� ���
	virtual unsigned int GetAlign()
	{
		return m_align;
	}

	//��ũ�� ��ǥ ���
	virtual D2D1_RECT_F GetScreenArea()
	{
		return m_screenArea;
	}

private:
	virtual DirectX::XMMATRIX Alignment(const unsigned int& screenWidth, const unsigned int& screenHeight, const unsigned int& flag)
	{
		DirectX::XMMATRIX mat;
		float x, y;

		switch (flag)
		{
		case ALIGNMENT_CENTER://���߾�
			//�����ǥ�� �� ������ǥ
			m_worldPosition = DirectX::XMFLOAT3(m_localPosition.x / m_scale.x, m_localPosition.y / m_scale.y, m_localPosition.z / m_scale.z);
			mat = DirectX::XMMatrixTranslation(m_worldPosition.x, m_worldPosition.y, m_worldPosition.z);
			break;
		case ALIGNMENT_LEFT://���� �߾�
			x = CalculateElementPosition(screenWidth, m_scale.x);
			m_worldPosition = DirectX::XMFLOAT3(-x + (m_localPosition.x / m_scale.x), (m_localPosition.y / m_scale.y), (m_localPosition.z / m_scale.z));
			mat = DirectX::XMMatrixTranslation(m_worldPosition.x, m_worldPosition.y, m_worldPosition.z);
			break;
		case ALIGNMENT_RIGHT://���� �߾�
			x = CalculateElementPosition(screenWidth, m_scale.x);
			m_worldPosition = DirectX::XMFLOAT3(x + (m_localPosition.x / m_scale.x), (m_localPosition.y / m_scale.y), (m_localPosition.z / m_scale.z));
			mat = DirectX::XMMatrixTranslation(m_worldPosition.x, m_worldPosition.y, m_worldPosition.z);
			break;
		case ALIGNMENT_TOP://��� �߾�
			y = CalculateElementPosition(screenHeight, m_scale.y);
			m_worldPosition = DirectX::XMFLOAT3((m_localPosition.x / m_scale.x), y + (m_localPosition.y / m_scale.y), (m_localPosition.z / m_scale.z));
			mat = DirectX::XMMatrixTranslation(m_worldPosition.x, m_worldPosition.y, m_worldPosition.z);
			break;
		case ALIGNMENT_BOTTOM://�ϴ� �߾�
			y = CalculateElementPosition(screenHeight, m_scale.y);
			m_worldPosition = DirectX::XMFLOAT3((m_localPosition.x / m_scale.x), -y + (m_localPosition.y / m_scale.y), (m_localPosition.z / m_scale.z));
			mat = DirectX::XMMatrixTranslation(m_worldPosition.x, m_worldPosition.y, m_worldPosition.z);
			break;
		case ALIGNMENT_LEFT_TOP://�»��
			x = CalculateElementPosition(screenWidth, m_scale.x);
			y = CalculateElementPosition(screenHeight, m_scale.y);
			m_worldPosition = DirectX::XMFLOAT3(-x + (m_localPosition.x / m_scale.x), y + (m_localPosition.y / m_scale.y), (m_localPosition.z / m_scale.z));
			mat = DirectX::XMMatrixTranslation(m_worldPosition.x, m_worldPosition.y, m_worldPosition.z);
			break;
		case ALIGNMENT_LEFT_BOTTOM://���ϴ�
			x = CalculateElementPosition(screenWidth, m_scale.x);
			y = CalculateElementPosition(screenHeight, m_scale.y);
			m_worldPosition = DirectX::XMFLOAT3(-x + (m_localPosition.x / m_scale.x), -y + (m_localPosition.y / m_scale.y), (m_localPosition.z / m_scale.z));
			mat = DirectX::XMMatrixTranslation(m_worldPosition.x, m_worldPosition.y, m_worldPosition.z);
			break;
		case ALIGNMENT_RIGHT_TOP://����
			x = CalculateElementPosition(screenWidth, m_scale.x);
			y = CalculateElementPosition(screenHeight, m_scale.y);
			m_worldPosition = DirectX::XMFLOAT3(x + (m_localPosition.x / m_scale.x), y + (m_localPosition.y / m_scale.y), (m_localPosition.z / m_scale.z));
			mat = DirectX::XMMatrixTranslation(m_worldPosition.x, m_worldPosition.y, m_worldPosition.z);
			break;
		case ALIGNMENT_RIGHT_BOTTOM://���ϴ�
			x = CalculateElementPosition(screenWidth, m_scale.x);
			y = CalculateElementPosition(screenHeight, m_scale.y);
			m_worldPosition = DirectX::XMFLOAT3(x + (m_localPosition.x / m_scale.x), -y + (m_localPosition.y / m_scale.y), (m_localPosition.z / m_scale.z));
			mat = DirectX::XMMatrixTranslation(m_worldPosition.x, m_worldPosition.y, m_worldPosition.z);
			break;
		default://ALIGNMENT_CENTER
			//�����ǥ�� �� ������ǥ
			m_worldPosition = DirectX::XMFLOAT3(m_localPosition.x / m_scale.x, m_localPosition.y / m_scale.y, m_localPosition.z / m_scale.z);
			mat = DirectX::XMMatrixTranslation(m_worldPosition.x, m_worldPosition.y, m_worldPosition.z);
		}

		return mat;
	}

	virtual float CalculateElementPosition(const unsigned int& resolution, const float& scale)
	{
		return static_cast<float>(resolution) / (scale * 2.0f) - 0.5f;
	}

	//���� ��� ������Ʈ
	virtual void UpdateWorldMatrix()
	{
		DirectX::XMMATRIX pos, rot, scale;

		m_worldMatrix = DirectX::XMMatrixIdentity();

		pos = Alignment(SCREEN_WIDTH, SCREEN_HEIGHT, m_align);
		rot = DirectX::XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
		scale = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);

		m_worldMatrix = DirectX::XMMatrixMultiply(m_worldMatrix, pos);
		m_worldMatrix = DirectX::XMMatrixMultiply(m_worldMatrix, scale);
		m_worldMatrix = DirectX::XMMatrixMultiply(m_worldMatrix, rot);

		return;
	}

	//��ũ�� ��ǥ ������Ʈ
	virtual void UpdateScreenArea()
	{
		m_screenArea = D2D1::RectF(
			m_worldPosition.x * m_scale.x + (SCREEN_WIDTH - m_scale.x) / 2,
			m_worldPosition.y * -m_scale.y + (SCREEN_HEIGHT - m_scale.y) / 2,
			m_worldPosition.x * m_scale.x + (SCREEN_WIDTH - m_scale.x) / 2 + m_scale.x,
			SCREEN_HEIGHT//������ ���� �������൵ ���ڰ� ��� �հ� �����Ƿ� �ִ�ġ�� ����
		);

		return;
	}

private:
	DirectX::XMMATRIX m_worldMatrix;
	DirectX::XMFLOAT3 m_localPosition;
	DirectX::XMFLOAT3 m_worldPosition;
	DirectX::XMFLOAT3 m_rotation;
	DirectX::XMFLOAT3 m_scale;
	D2D1_RECT_F m_screenArea;
	unsigned int m_align;
};

#endif