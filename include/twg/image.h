#ifndef TWG_IMAGE_INCLUDED
#define TWG_IMAGE_INCLUDED

#include <string>

#include "twg/basics.h"
#include "twg/point.h"
#include "twg/color.h"

namespace twg
{

	class ImageWin;
	class ImageBase;

	//-------------------------------------------------------------------------
	class ImageWin
	{
	public:
		ImageWin();
		ImageWin(HDC hdc);
		~ImageWin();

		HDC getHdc(void);
		void assign(HDC hdc);
		void assignScreen(void);

		virtual int32u width(void);
		virtual int32u height(void);
		virtual Point_i size(void);

		void copyTo(ImageWin *dst, 
					Point_i dstStart, 
					Point_i srcStart,
					Point_i srcSize);
	protected:
		HDC		m_hdc;
	};

	//-------------------------------------------------------------------------
	class ImageBase : public ImageWin
	{
	public:
		ImageBase(Point_i size);	
		~ImageBase();

		void resize(Point_i newSize);
	
		Color* buf(void);
		int32u width(void);
		int32u height(void);
		Point_i size(void);

		void clear(Color bk = White);
		
		Color& getPixel(int32 x, int32 y); // Имеет проверку на выход за границы
		Color& operator[](Point_i pos);
	protected:
		Color*	m_buf;
		int32u	m_width;
		int32u	m_height;

		HBITMAP m_hbmp;
	};

	//-------------------------------------------------------------------------
	void loadFromBmp(ImageBase* img, std::wstring fileName);
	void saveToBmp(ImageBase* img, std::wstring fileName, bool is32bitBmp = false);
	
}

#endif // TWG_IMAGE_INCLUDED