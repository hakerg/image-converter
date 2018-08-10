#pragma once
#include <windows.h>

class ScreenCapturer
{
	RGBQUAD* data;
	HDC hDesktopDC, hMyDC;
	HBITMAP hBitmap;
	BITMAPINFO bi;
	int width, height, size;

public:
	ScreenCapturer(int _width, int _height);
	~ScreenCapturer();
	void capture();
	RGBQUAD* getData();
};

