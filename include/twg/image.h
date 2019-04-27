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

		virtual int32u width(void) const;
		virtual int32u height(void) const;
		virtual Point_i size(void) const;

		void copyTo(ImageWin *dst, 
					Point_i dstStart,  
					Point_i srcStart,
					Point_i srcSize) const;

		void drawTo(ImageWin* dst, 
					Point_i dstStart,
					Point_i srcStart,
					Point_i srcSize,
					Point_i dstSize) const;
	protected:
		HDC		m_hdc;
	};

	//-------------------------------------------------------------------------
	class ImageBase : public ImageWin
	{
	public:
		ImageBase(Point_i size);
		ImageBase(const ImageBase& img); // Конструктор копирования. Копируется размер, создается новый буфер, туда рисуется все, что есть в этом.
		~ImageBase();

		void assign(ImageBase* img); // Режим, когда можно создать новый объект, который будет рисовать в тот же буфер. При удалении объекта-ссылки не удаляется изначальный буфер. При ресайзинге изменяется изначальный буфер. При этом нельзя ресайзить изначальный буфер, иначе поведение будет не определено.

		virtual void resize(Point_i newSize);
	
		Color* buf(void);
		const Color* buf(void) const;
		int32u width(void) const;
		int32u height(void) const;
		Point_i size(void) const;

		void clear(Color bk = White);
		
		Color& getPixel(const Point_i& pos);
		const Color& getPixel(const Point_i& pos) const;

		Color& operator[](const Point_i& pos);
		const Color& operator[](const Point_i& pos) const;
	protected:
		ImageBase* 	m_assigned;
		Color*		m_buf;
		int32u		m_width;
		int32u		m_height;
		HBITMAP 	m_hbmp;

		using ImageWin::assignScreen;
		using ImageWin::assign;
	};

	//-------------------------------------------------------------------------
	void loadFromBmp(ImageBase* img, std::wstring fileName);
	void saveToBmp(ImageBase* img, std::wstring fileName, bool is32bitBmp = false);
	void loadFromPngJpg(ImageBase* img, std::wstring fileName);
	void saveToPng(ImageBase* img, std::wstring fileName, bool is32bitPng = true);

	//-------------------------------------------------------------------------
	class ImageGif
	{
	public:
		void start(Point_i size, const std::string& filename);
		void process(const ImageBase& img, int delay_ms);
		void end(void);
	private:
		int width, height;
		void* writer;
		uint32_t* img2;
	};


//=============================================================================
//=============================================================================
//=============================================================================

//-----------------------------------------------------------------------------
inline HDC ImageWin::getHdc(void) {
	return m_hdc;
}

//-----------------------------------------------------------------------------
inline int32u ImageWin::width(void) const {
	return size().x;
}

//-----------------------------------------------------------------------------
inline int32u ImageWin::height(void) const {
	return size().y;
}

//-----------------------------------------------------------------------------
inline Color& ImageBase::getPixel(const Point_i& pos) {
	if (pos.x > m_width || pos.x < 0 || pos.y > m_height || pos.y < 0)
		// Вызвать исключение
		return *m_buf;
	else
		return m_buf[pos.x + m_width * pos.y];
}

//-----------------------------------------------------------------------------
inline Color& ImageBase::operator[](const Point_i& pos) {
	return m_buf[pos.x + m_width * pos.y];
}

//-----------------------------------------------------------------------------
inline const Color& ImageBase::getPixel(const Point_i& pos) const {
	if (pos.x > m_width || pos.x < 0 || pos.y > m_height || pos.y < 0)
		// Вызвать исключение
		return *m_buf;
	else
		return m_buf[pos.x + m_width * pos.y];
}

//-----------------------------------------------------------------------------
inline const Color& ImageBase::operator[](const Point_i& pos) const {
	return m_buf[pos.x + m_width * pos.y];
}

//-----------------------------------------------------------------------------
inline Color* ImageBase::buf(void) {
	return m_buf;
}

//-----------------------------------------------------------------------------
inline const Color* ImageBase::buf(void) const {
	return m_buf;
}

//-----------------------------------------------------------------------------
inline int32u ImageBase::width(void) const {
	return m_width;
}

//-----------------------------------------------------------------------------
inline int32u ImageBase::height(void) const {
	return m_height;
}

//-----------------------------------------------------------------------------
inline Point_i ImageBase::size(void) const {
	return Point_i(m_width, m_height);
}
	
}

#endif // TWG_IMAGE_INCLUDED