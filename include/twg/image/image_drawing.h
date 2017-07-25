#ifndef TWG_IMAGE_DRAWING_INCLUDED
#define TWG_IMAGE_DRAWING_INCLUDED

#include "twg/basics.h"
#include "twg/image.h"

namespace twg
{

	class 	Pen;
	class 	Brush;
	class 	TextStyle;
	enum 	TextFlag;

	//-------------------------------------------------------------------------
	class ImageDrawing : ImageBase
	{
	public:
		virtual void setPen(Pen pen) = 0;
		virtual void setBrush(Brush brush) = 0;
		virtual void setTextStyle(int8u width, 
								  std::string name, 
								  TextFlag flags = TEXT_NONE) = 0;

		virtual void drawTo(ImageWin *dst, 
							Point_i dstStart, 
							Point_i srcStart,
							Point_i srcSize); // Used AlphaBlend.

		virtual Pen 		getPen(void) = 0;
		virtual Brush 		getBrush(void) = 0;
		virtual TextStyle 	getTextStyle(void) = 0;
		virtual Point_d 	getTextSize(std::string) = 0;

		virtual void drawPolygon(std::vector<Point_d> points) = 0;
		virtual void drawPolyline(std::vector<Point_d> points, 
								  bool isRoundJoin) = 0;
		virtual void drawLine(Point_d a, Point_d b) = 0;

		virtual void fillFlood(Point_d pos) = 0;

		virtual void drawText(Point_d pos, std::string text) = 0;
	protected:
		Pen 		m_pen;
		Brush 		m_brush;
		TextStyle 	m_textStyle;
	};

	class ImageDrawing_aa : ImageDrawing {};
	class ImageDrawing_win : ImageDrawing {};

	//-------------------------------------------------------------------------
	std::vector<Point_d> computeEllipse(Point_d pos, Point_d radius);
	std::vector<Point_d> computeArc(Point_d c, 
								 double radius, 
								 double startAngle,
								 double endAngle);
	/// @todo write other primitives ...

	//-------------------------------------------------------------------------
	class Pen
	{
	public:
		Pen(int32u thick, Color clr) : thick(thick), clr(clr), img(nullptr) {};
		Pen(int32u thick, ImageBase* img) : thick(thick), clr(Transparent), 
			img(img) {};

		int32u 		thick;
		Color 		clr;
		ImageBase*	img;
	};

	class Brush
	{
	public:
		Brush(Color clr) : clr(clr), img(nullptr) {};
		Brush(ImageBase* img) : clr(Transparent), img(img) {};

		Color 		clr;
		ImageBase*	img;
	};

	enum TextFlag 
	{
		TEXT_NONE		= 0,
		TEXT_BOLD 		= 1,
		TEXT_ITALIC 	= 2,
		TEXT_UNDERLINE 	= 4,
		TEXT_STRIKE 	= 8
	}

	class TextStyle
	{
	public:
		TextStyle(int8u width, std::string name, TextFlag  flags = TEXT_NONE) :
			width(width), name(name), flags(flags) {};

		int8u		width;
		std::string	name;
		TextFlag 	flags;
	};
	
}

#endif // TWG_IMAGE_DRAWING_INCLUDED