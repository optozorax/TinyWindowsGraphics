#ifndef TWG_IMAGE_INCLUDED
#define TWG_IMAGE_INCLUDED

#include <string>

#include "twg_basics.h"
#include "twg_point.h"
#include "twg_color.h"

namespace twg
{

	class ImageWin
	{
	public:
		ImageWin();
		ImageWin(HDC hdc);
		~ImageWin();

		virtual void copyTo(ImageWin *dst, 
							Point_i dstStart, 
							Point_i srcStart,
							Point_i srcSize);

		HDC 	getHdc(void);
	protected:
		HDC		m_hdc;
	}

	class ImageBase : ImageWin
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
		
		Color* getPixel(int32 x, int32 y);
		Color* operator[](int32 x, int32 y);
	protected:
		Color*	m_buf;
		int32u	m_width;
		int32u	m_height;

		HBMP 	m_hbmp;
	};

	//-------------------------------------------------------------------------
	void loadFromBmp(ImageBase *img, std::string fileName);
	void saveToBmp(ImageBase *img, std::string fileName, bool is32bit = false);
	
}

#endif // TWG_IMAGE_INCLUDED