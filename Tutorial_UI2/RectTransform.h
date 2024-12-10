#ifndef _RECT_TRANSFORM_
#define _RECT_TRANSFORM_

#include <DirectXMath.h>

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
		m_screenX = 0;
		m_screenY = 0;
	}

	virtual ~RectTransform()
	{
	}

public:
	//���� ��� �ʱ�ȭ
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

		//��ũ�� ��ǥ�� ������Ʈ
		m_screenX = m_worldPosition.x * m_scale.x;
		m_screenY = m_worldPosition.y * -m_scale.y;

		return;
	}

	//���� ��� ��������
	virtual DirectX::XMMATRIX GetWorldMatrix()
	{
		return m_worldMatrix;
	}

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

	//��� ��ġ ���
	virtual DirectX::XMFLOAT3 GetLocalPosition()
	{
		return m_localPosition;
	}

	//���� ��ġ ���
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
	virtual void GetScreenPos(float& x, float& y)
	{
		x = m_screenX;
		y = m_screenY;
		return;
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

private:
	DirectX::XMMATRIX m_worldMatrix;
	DirectX::XMFLOAT3 m_localPosition;
	DirectX::XMFLOAT3 m_worldPosition;
	DirectX::XMFLOAT3 m_rotation;
	DirectX::XMFLOAT3 m_scale;
	unsigned int m_align;
	float m_screenX, m_screenY;
};

#endif