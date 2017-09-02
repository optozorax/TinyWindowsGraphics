#include <windows.h>
#include "EasyBMP.h"
#include "twg/image.h"
#include "twg/color.h"

namespace twg
{

//-----------------------------------------------------------------------------
ImageWin::ImageWin() {
	m_hdc = 0;
}

//-----------------------------------------------------------------------------
ImageWin::ImageWin(HDC hdc) {
	m_hdc = hdc;
}

//-----------------------------------------------------------------------------
ImageWin::~ImageWin() {
	if (m_hdc != 0) DeleteObject(m_hdc);
}

//-----------------------------------------------------------------------------
void ImageWin::copyTo(ImageWin* dst, 
					  Point_i dstStart, 
					  Point_i srcStart,
					  Point_i srcSize) const {
	BitBlt(dst->m_hdc, dstStart.x, dstStart.y, srcSize.x, srcSize.y, m_hdc, srcStart.x, srcStart.y, SRCCOPY);
}

//-----------------------------------------------------------------------------
void ImageWin::drawTo(ImageWin* dst, 
					  Point_i dstStart, 
					  Point_i srcStart,
					  Point_i srcSize) const {
	BLENDFUNCTION blendFn = {};
	blendFn.BlendOp = AC_SRC_OVER;
	blendFn.BlendFlags = 0;
	blendFn.SourceConstantAlpha = 255;
	blendFn.AlphaFormat = 0x1;

	AlphaBlend(dst->m_hdc, 
			   dstStart.x, dstStart.y,
			   srcSize.x, srcSize.y,
			   m_hdc,
			   srcStart.x, srcStart.y,
			   srcSize.x, srcSize.y,
			   blendFn);
}


//-----------------------------------------------------------------------------
void ImageWin::assign(HDC hdc) {
	if (m_hdc != 0) DeleteDC(m_hdc);
	m_hdc = hdc;
}

//-----------------------------------------------------------------------------
void ImageWin::assignScreen(void) {
	if (m_hdc != 0) DeleteDC(m_hdc);
	m_hdc = CreateDC(L"DISPLAY", NULL, NULL, NULL);
}

//-----------------------------------------------------------------------------
Point_i ImageWin::size(void) const {
	BITMAP hbmp;
	memset(&hbmp, 0, sizeof(BITMAP));

	HGDIOBJ hBitmap = GetCurrentObject(m_hdc, OBJ_BITMAP);
	GetObject(hBitmap, sizeof(BITMAP), &hbmp);

	return Point_i(hbmp.bmWidth, hbmp.bmHeight);
}

//-----------------------------------------------------------------------------
ImageBase::ImageBase(Point_i sz) : m_hbmp(0) {
	m_assigned = nullptr;
	resize(sz);
}

//-----------------------------------------------------------------------------
ImageBase::ImageBase(const ImageBase& img) : m_hbmp(0) {
	m_assigned = nullptr;
	resize(img.size());
	img.copyTo(this, Point_i(0, 0), Point_i(0, 0), img.size());
}

//-----------------------------------------------------------------------------
ImageBase::~ImageBase() {
	if (m_assigned == nullptr) {
		DeleteObject(m_hbmp);
		DeleteDC(m_hdc);
	}
	m_hdc = 0;
}

//-----------------------------------------------------------------------------
void ImageBase::assign(ImageBase* img) {
	DeleteObject(m_hbmp);
	DeleteDC(m_hdc);
	m_assigned = img;
	m_buf = img->m_buf;
	m_width = img->m_width;
	m_height = img->m_height;
	m_hbmp = img->m_hbmp;
	m_hdc = img->m_hdc;	
}

//-----------------------------------------------------------------------------
void ImageBase::resize(Point_i newSize) {
	if (m_assigned != nullptr)
		m_assigned->resize(newSize);
	else {
		DeleteObject(m_hbmp);
		DeleteDC(m_hdc);

		m_width = newSize.x;
		m_height = newSize.y;

		BITMAPINFO bmi = {};
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
		bmi.bmiHeader.biWidth = newSize.x;
		bmi.bmiHeader.biHeight = -newSize.y;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biSizeImage = newSize.x * newSize.y * 4;
		
		VOID *pvBits;
		m_hdc = CreateCompatibleDC(NULL);
		m_hbmp = CreateDIBSection(m_hdc, &bmi, DIB_RGB_COLORS, &pvBits, nullptr, 0);
		SetGraphicsMode(m_hdc, GM_ADVANCED);
		
		m_buf = (Color*)(pvBits);
		DeleteObject(SelectObject(m_hdc, m_hbmp));
	}
}

//-----------------------------------------------------------------------------
void ImageBase::clear(Color bk) {
	for (int32u i = 0; i < m_width*m_height; ++i)
			m_buf[i] = bk;
}

//-----------------------------------------------------------------------------
void loadFromBmp(ImageBase* img, std::wstring fileName) {
	BMP AnImage;
	AnImage.ReadFromFile(fileName.c_str());

	img->resize(Point_i(AnImage.TellWidth(), AnImage.TellHeight()));

	if (AnImage.TellBitDepth() <= 24) {
		AnImage.SetBitDepth(32);
		for (size_t i = 0; i < AnImage.TellWidth(); i++) {
			for (size_t j = 0; j < AnImage.TellHeight(); j++) {
				RGBApixel pix = AnImage.GetPixel(i, j);
				pix.Alpha = 255;

				(*img)[Point_i(i, j)] = rgb(pix.Red, pix.Green, pix.Blue);
			}
		}
	} else {
		for (size_t i = 0; i < AnImage.TellWidth(); i++) {
			for (size_t j = 0; j < AnImage.TellHeight(); j++) {
				RGBApixel pix = AnImage.GetPixel(i, j);

				(*img)[Point_i(i, j)] = rgba(pix.Red, pix.Green, pix.Blue, pix.Alpha);
			}
		}
	}
}

//-----------------------------------------------------------------------------
void saveToBmp(ImageBase* img, std::wstring fileName, bool is32bitBmp) {
	if (is32bitBmp) {
		BMP AnImage;
		AnImage.SetBitDepth(32);
		AnImage.SetSize(img->width(), img->height());

		RGBApixel pix;
		for (size_t i = 0; i < AnImage.TellWidth(); i++) {
			for (size_t j = 0; j < AnImage.TellHeight(); j++) {
				Color mypix = (*img)[Point_i(i,j)];

				pix.Alpha = getAlpha(mypix);
				pix.Blue = getBlue(mypix);
				pix.Red = getRed(mypix);
				pix.Green = getGreen(mypix);

				AnImage.SetPixel(i, j, pix);
			}
		}

		AnImage.WriteToFile(fileName.c_str());
	} else {
		BMP AnImage;
		AnImage.SetSize(img->width(), img->height());

		RGBApixel pix;
		for (size_t i = 0; i < AnImage.TellWidth(); i++) {
			for (size_t j = 0; j < AnImage.TellHeight(); j++) {
				Color mypix = (*img)[Point_i(i,j)];

				pix.Blue = getBlue(mypix);
				pix.Red = getRed(mypix);
				pix.Green = getGreen(mypix);

				AnImage.SetPixel(i, j, pix);
			}
		}

		AnImage.WriteToFile(fileName.c_str());
	}
}

}