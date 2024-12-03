#ifndef _EVENT_CLASS_
#define _EVENT_CLASS_

#include <vector>
#include <functional>

enum struct UIEvent
{
	EVENT_DEFAULT,
	EVENT_TOGGLE_TEST_CANVAS,
	EVENT_SIZE,//이벤트 개수 //[]배열 크기 할당하기 위함
};

enum struct InputEvent
{
	EVENT_DEFAULT,
	MOUSE_LEFT_PRESSED,
	MOUSE_LEFT_RELEASED,
	EVENT_SIZE,//이벤트 개수
};

enum struct SceneEvent
{
	EVENT_DEFAULT,
	EVENT_SIZE,//이벤트 개수
};

class EventClass
{
public:
	static EventClass& GetInstance() 
	{
		static EventClass instance;

		return instance;
	}

	//복사 생성자랑 대입 연산 방지
	EventClass(const EventClass&) = delete;
	void operator=(const EventClass&) = delete;

public:
	//이벤트 발생 시, 실행할 함수를 함수 포인터 배열에 등록하는 함수
	void Subscribe(UIEvent event, std::function<void()> func)
	{
		m_uiEvents.push_back(func);
	}

	//이벤트 발생 시,  해당 이벤트의 함수들을 실행
	void Publish(UIEvent event)
	{
		for (UINT i = 0; i < m_uiEvents.size(); i++)
		{
			m_uiEvents[i]();
		}
	}

private:
	//생성자를 private에 두어 객체 생성 방지
	EventClass()
	{
	}

	~EventClass()
	{
	}

private:
	std::vector<std::function<void()>> m_uiEvents;
};

#endif