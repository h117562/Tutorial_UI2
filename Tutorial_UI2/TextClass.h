#ifndef _TEXTCLASS_H_
#define _TEXTCLASS_H_

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

#include <dwrite.h>
#include <d2d1.h>

class TextClass
{
public:
	TextClass();
	~TextClass();
	TextClass(const TextClass&);

	bool Initialize(IDXGISwapChain*);
	void Shutdown();

	void RenderText(const wchar_t* text, float x, float y, float width, float height);
	void RenderText(const wchar_t* text, float x, float y, float width, float height, IDWriteTextFormat*, ID2D1SolidColorBrush*);
	void BeginDraw();
	void EndDraw();

private:
	ID2D1Factory* m_d2dFactory;
	IDWriteFactory* m_dwFactory;
	IDWriteTextFormat* m_defaultFormat;
	ID2D1RenderTarget* m_renderTarget;
	ID2D1SolidColorBrush* m_defaultBrush;
};

#endif