#ifndef _RECT_TRANSFORM_
#define _RECT_TRANSFORM_

#include <DirectXMath.h>

enum ALIGNMENT//정렬 플래그
{
	ALIGNMENT_CENTER,//정중앙
	ALIGNMENT_LEFT,//좌측 중앙
	ALIGNMENT_RIGHT,//우측 중앙
	ALIGNMENT_TOP,//상단 중앙
	ALIGNMENT_BOTTOM,//하단 중앙
	ALIGNMENT_LEFT_TOP,//좌상단
	ALIGNMENT_LEFT_BOTTOM,//좌하단
	ALIGNMENT_RIGHT_TOP,//우상단
	ALIGNMENT_RIGHT_BOTTOM,//우하단
};

class RectTransform//요소의 위치 수정 및 정렬 기능을 제공하는 추상 클래스
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

	//위치 설정
	virtual void SetPosition(float x, float y, float z)
	{
		m_position = DirectX::XMFLOAT3(x, y, z);
	}

	//회전 설정
	virtual void SetRotation(float x, float y, float z)
	{
		m_rotation = DirectX::XMFLOAT3(x, y, z);
	}

	//크기 설정
	virtual void SetScale(float x, float y, float z)
	{
		m_rotation = DirectX::XMFLOAT3(x, y, z);
	}

	//여백 설정
	virtual void SetMargin(float x, float y)
	{
		m_margin = DirectX::XMFLOAT2(x, y);
	}

	//정렬 설정
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