#ifndef _TEXT_FIELD_
#define _TEXT_FIELD_

#include "RectTransform.h"
#include "RectTexture2D.h"

class Textfield : public RectTransform , public RectTexture2D
{
public:
	Textfield();
	~Textfield();

	bool Initialize();
	void Render();
	void Shutdown();

private:
	
};

#endif
