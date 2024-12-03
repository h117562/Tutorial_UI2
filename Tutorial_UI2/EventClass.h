#ifndef _EVENT_CLASS_
#define _EVENT_CLASS_

#include <vector>
#include <functional>

enum struct UIEvent
{
	EVENT_DEFAULT,
	EVENT_TOGGLE_TEST_CANVAS,
	EVENT_SIZE,//�̺�Ʈ ���� //[]�迭 ũ�� �Ҵ��ϱ� ����
};

enum struct InputEvent
{
	EVENT_DEFAULT,
	MOUSE_LEFT_PRESSED,
	MOUSE_LEFT_RELEASED,
	EVENT_SIZE,//�̺�Ʈ ����
};

enum struct SceneEvent
{
	EVENT_DEFAULT,
	EVENT_SIZE,//�̺�Ʈ ����
};

class EventClass
{
public:
	static EventClass& GetInstance() 
	{
		static EventClass instance;

		return instance;
	}

	//���� �����ڶ� ���� ���� ����
	EventClass(const EventClass&) = delete;
	void operator=(const EventClass&) = delete;

public:
	//�̺�Ʈ �߻� ��, ������ �Լ��� �Լ� ������ �迭�� ����ϴ� �Լ�
	void Subscribe(UIEvent event, std::function<void()> func)
	{
		m_uiEvents.push_back(func);
	}

	//�̺�Ʈ �߻� ��,  �ش� �̺�Ʈ�� �Լ����� ����
	void Publish(UIEvent event)
	{
		for (UINT i = 0; i < m_uiEvents.size(); i++)
		{
			m_uiEvents[i]();
		}
	}

private:
	//�����ڸ� private�� �ξ� ��ü ���� ����
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