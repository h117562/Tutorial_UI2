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
		//기본 설정 데이터
		m_position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_scale = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_margin = DirectX::XMFLOAT2(0.0f, 0.0f);
		m_align = ALIGNMENT_CENTER;
	}

	~RectTransform()
	{
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

	//위치 얻기
	virtual DirectX::XMFLOAT3 GetPosition()
	{
		return m_position;
	}

	//회전 얻기
	virtual DirectX::XMFLOAT3 GetRotation()
	{
		return m_position;
	}

	//크기 얻기
	virtual DirectX::XMFLOAT3 GetScale()
	{
		return m_scale;
	}

	//여백 얻기
	virtual DirectX::XMFLOAT2 GetMargin()
	{
		return m_margin;
	}

	//정렬 얻기
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

};

#endif