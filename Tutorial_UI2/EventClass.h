
class EventClass
{
public:
	EventClass();
	~EventClass();

	void Set(const int i)
	{
		a = i;
	}

	int Get()
	{
		return a;
	}

private:
	int a = 0;
};