#include "ScreenCapturer.h"



ScreenCapturer::ScreenCapturer(int _width, int _height)
{
	width = _width;
	height = _height;
	size = width * height;
	hDesktopDC = GetDC(NULL);
	hMyDC = CreateCompatibleDC(hDesktopDC);
	memset(&bi, 0, sizeof(&bi));
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biWidth = width;
	bi.bmiHeader.biHeight = -height;
	hBitmap = CreateCompatibleBitmap(hDesktopDC, width, height);
	SelectObject(hMyDC, hBitmap);
	SetStretchBltMode(hMyDC, HALFTONE);
	data = new RGBQUAD[size];
}


ScreenCapturer::~ScreenCapturer()
{
	DeleteObject(hBitmap);
	ReleaseDC(NULL, hDesktopDC);
	DeleteDC(hMyDC);
	delete[] data;
}

void ScreenCapturer::capture()
{
	StretchBlt(hMyDC, 0, 0, width, height, hDesktopDC, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SRCCOPY);
	GetDIBits(hMyDC, hBitmap, 0, height, data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
}

RGBQUAD * ScreenCapturer::getData()
{
	return data;
}