#ifndef _EVENT_CLASS_
#define _EVENT_CLASS_

#define TO_INT(value) (static_cast<int>(value)) //enum long Ÿ���� int�� ��ȯ���ֱ� ���� �� ������ �����ϰ� ���̵��� ���� //enum struct�� []���ȣ�� ������ �ٷ� ����

#include <vector>
#include <functional>

enum struct UI_EVENT
{
	DEFAULT,
	TOGGLE_DEBUG_MODE,
	TOGGLE_TEST_CANVAS,
	EVENT_COUNT,//�̺�Ʈ ���� //[]�迭 ũ�� �Ҵ��ϱ� ����
};

enum struct SCENE_EVENT
{
	DEFAULT,
	EVENT_COUNT,//�̺�Ʈ ����
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

private:
	//�����ڸ� private�� �ξ� ��ü ���� ����
	EventClass()
	{
	}

	~EventClass()
	{
	}

public:

	//�̺�Ʈ �߻� ��, ������ �Լ��� �Լ� ������ �迭�� ����ϴ� �Լ�
	void Subscribe(UI_EVENT event, std::function<void()> func)
	{
		m_uiEvents[TO_INT(event)].push_back(func);
	}

	//�̺�Ʈ �߻� ��,  �ش� �̺�Ʈ�� �Լ����� ����
	void Publish(UI_EVENT event)
	{
		for (UINT i = 0; i < m_uiEvents[TO_INT(event)].size(); i++)
		{
			m_uiEvents[TO_INT(event)][i]();
		}
	}

private:
	std::vector<std::function<void()>> m_uiEvents[TO_INT(UI_EVENT::EVENT_COUNT)];
	std::vector<std::function<void()>> m_sceneEvents[TO_INT(SCENE_EVENT::EVENT_COUNT)];
};

#endif