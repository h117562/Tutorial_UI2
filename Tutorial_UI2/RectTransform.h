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
public:

	RectTransform()
	{
		m_position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_scale = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_margin = DirectX::XMFLOAT2(0.0f, 0.0f);
		m_align = ALIGNMENT_CENTER;

		m_vertices = new DirectX::XMFLOAT3[4];

		m_vertices[0] = DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f);
		m_vertices[1] = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);
		m_vertices[2] = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f);
		m_vertices[3] = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f);

		m_indices = new unsigned int[6];

		m_indices[0] = 0;
		m_indices[1] = 3;
		m_indices[2] = 2;
		m_indices[3] = 3;
		m_indices[4] = 0;
		m_indices[5] = 1;

	}

	~RectTransform()
	{
		if (m_vertices)
		{
			delete[] m_vertices;
			m_vertices = nullptr;
		}

		if (m_indices)
		{
			delete[] m_indices;;
			m_indices = nullptr;
		}
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
		m_rotation = DirectX::XMFLOAT3(x, y, z);
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

	virtual DirectX::XMFLOAT3 GetPosition()
	{
		return m_position;
	}

	virtual DirectX::XMFLOAT3 GetRotation()
	{
		return m_position;
	}

	virtual DirectX::XMFLOAT3 GetScale()
	{
		return m_scale;
	}

	virtual DirectX::XMFLOAT2 GetMargin()
	{
		return m_margin;
	}

	virtual unsigned int GetAlign()
	{
		return m_align;
	}

private:
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_rotation;
	DirectX::XMFLOAT3 m_scale;
	DirectX::XMFLOAT2 m_margin;
	unsigned int m_align;

	DirectX::XMFLOAT3* m_vertices;
	unsigned int* m_indices;

};

#endif