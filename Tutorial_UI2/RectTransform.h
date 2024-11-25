#ifndef _RECT_TRANSFORM_
#define _RECT_TRANSFORM_

#include <DirectXMath.h>

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
		m_position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_scale = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_margin = DirectX::XMFLOAT2(0.0f, 0.0f);
		m_align = ALIGNMENT_CENTER;
		m_worldMatrix = DirectX::XMMatrixIdentity();
	}

	~RectTransform()
	{
	}

public:
	//���� ��� �ʱ�ȭ
	virtual void UpdateWorldMatrix(const unsigned int& screenWidth, const unsigned int& screenHeight)
	{
		DirectX::XMMATRIX pos, rot, scale;

		m_worldMatrix = DirectX::XMMatrixIdentity();
		
		pos = Alignment(screenWidth, screenHeight, m_align);
		rot = DirectX::XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
		scale = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);

		m_worldMatrix = DirectX::XMMatrixMultiply(m_worldMatrix, pos);
		m_worldMatrix = DirectX::XMMatrixMultiply(m_worldMatrix, scale);
		m_worldMatrix = DirectX::XMMatrixMultiply(m_worldMatrix, rot);
	}

	//���� ��� ��������
	virtual DirectX::XMMATRIX GetWorldMatrix()
	{
		return m_worldMatrix;
	}

	//��ġ ����
	virtual void SetPosition(float x, float y, float z)
	{
		m_position = DirectX::XMFLOAT3(x, y, z);
	}

	//ȸ�� ����
	virtual void SetRotation(float x, float y, float z)
	{
		m_rotation = DirectX::XMFLOAT3(x, y, z);
	}

	//ũ�� ����
	virtual void SetScale(float x, float y, float z)
	{
		m_scale = DirectX::XMFLOAT3(x, y, z);
	}


	//���� ����
	virtual void SetMargin(float x, float y)
	{
		m_margin = DirectX::XMFLOAT2(x, y);
	}

	//���� ����
	virtual void SetAlign(unsigned int flag)
	{
		m_align = flag;
	}

	//��ġ ���
	virtual DirectX::XMFLOAT3 GetPosition()
	{
		return m_position;
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
	virtual DirectX::XMFLOAT2 GetMargin()
	{
		return m_margin;
	}

	//���� ���
	virtual unsigned int GetAlign()
	{
		return m_align;
	}

private:
	virtual DirectX::XMMATRIX Alignment(const unsigned int& screenWidth, const unsigned int& screenHeight, unsigned int flag)
	{
		DirectX::XMMATRIX pos;
		float x, y;

		switch (flag)
		{
		case ALIGNMENT_CENTER://���߾�
			pos = DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
			break;
		case ALIGNMENT_LEFT://���� �߾�
			x = (float)screenWidth / (m_scale.x * 2.0f) - 0.5f;
			pos = DirectX::XMMatrixTranslation(-x + m_position.x, m_position.y, m_position.z);
			break;
		case ALIGNMENT_RIGHT://���� �߾�
			x = (float)screenWidth / (m_scale.x * 2.0f) - 0.5f;
			pos = DirectX::XMMatrixTranslation(x + m_position.x, m_position.y, m_position.z);
			break;
		case ALIGNMENT_TOP://��� �߾�
			y = (float)screenHeight / (m_scale.y * 2.0f) - 0.5f;
			pos = DirectX::XMMatrixTranslation(m_position.x, y + m_position.y, m_position.z);
			break;
		case ALIGNMENT_BOTTOM://�ϴ� �߾�
			y = (float)screenHeight / (m_scale.y * 2.0f) - 0.5f;
			pos = DirectX::XMMatrixTranslation(m_position.x, -y + m_position.y, m_position.z);
			break;

			//ALIGNMENT_LEFT_TOP,//�»��
			//ALIGNMENT_LEFT_BOTTOM,//���ϴ�
			//ALIGNMENT_RIGHT_TOP,//����
			//ALIGNMENT_RIGHT_BOTTOM,//���ϴ�
		}

		return pos;
	}

private:
	DirectX::XMMATRIX m_worldMatrix;
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_rotation;
	DirectX::XMFLOAT3 m_scale;
	DirectX::XMFLOAT2 m_margin;
	unsigned int m_align;
};

#endif