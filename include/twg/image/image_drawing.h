#ifndef TWG_IMAGE_DRAWING_INCLUDED
#define TWG_IMAGE_DRAWING_INCLUDED

#include "twg/basics.h"
#include "twg/image.h"
#include "twg/image/image_agg.h"

namespace twg
{

	class 	Pen;
	class 	Brush;
	class 	TextStyle;
	enum 	TextFlag : int32u;
	class 	Polygon_d;
	class 	ImageDrawing;
	class 	ImageDrawing_aa;
	class 	ImageDrawing_win;

	//-------------------------------------------------------------------------
	class Pen
	{
	public:
		Pen() : thick(1), clr(Black), img(nullptr) {}
		Pen(double thick, Color clr) : thick(thick), clr(clr), img(nullptr) {};
		Pen(double thick, ImageBase* img) : thick(thick), clr(Transparent), 
			img(img) {};

		double 		thick;
		Color 		clr;
		ImageBase*	img;
	};

	class Brush
	{
	public:
		Brush() : clr(White), img(nullptr) {}
		Brush(Color clr) : clr(clr), img(nullptr) {}
		Brush(ImageBase* img) : clr(Transparent), img(img) {};

		Color 		clr;
		ImageBase*	img;
	};

	class TextStyle
	{
	public:
		TextStyle() : width(12), name(L"Consolas"), flags(0) {}
		TextStyle(int32 width, std::wstring name, TextFlag  flags) :
			width(width), name(name), flags(flags) {};

		int32			width;
		std::wstring	name;
		TextFlag 		flags;
	};

	enum TextFlag : int32u
	{
		TEXT_NONE		= 0,
		TEXT_BOLD 		= 1,
		TEXT_ITALIC 	= 2,
		TEXT_UNDERLINE 	= 4,
		TEXT_STRIKE 	= 8
	};

	//-------------------------------------------------------------------------
	class Polygon_d
	{
	public:
		Polygon_d() {}
		Polygon_d(std::vector<Point_d> array) : array(array) {}

		std::vector<Point_d> array;

		Polygon_d& move(Point_d diff);
		Polygon_d& rotate(double angle, Point_d center);
		Polygon_d& scale(Point_d scale);
		Polygon_d& toBasis(Point_d newOX, Point_d newOY);
		Polygon_d& fromBasis(Point_d oldOX, Point_d oldOY);

		Polygon_d& flipAxis(Point_d axis); // You can mirror coords by OX(1, 0)
	};

	Polygon_d computeEllipse(Point_d radius);
	Polygon_d computeArc(double radius, 
						 double startAngle,
						 double endAngle);
	Polygon_d computePie(double radius, 
						 double startAngle,
						 double endAngle);
	Polygon_d computeRoundRect(Point_d size, // Border-radius:
							   double rLU, // Left-Up
							   double rRU, // Right-Up
							   double rRD, // Right-Down
							   double rLD); // Left-Down
	Polygon_d computeRegularPolygon(int32u n, double radius);

	//-------------------------------------------------------------------------
	class ImageDrawing : virtual public ImageBase
	{
	public:
		ImageDrawing(Point_i size) : ImageBase(size) {}

		virtual ~ImageDrawing() {}

		virtual void setPen(Pen pen) {}
		virtual void setBrush(Brush brush) {}
		virtual void setTextStyle(TextStyle style) {}

			// Rectangular drawing
		virtual void drawPTo(ImageBase* dst, Polygon_d rect) {} // ONLY 4 POINTS
			// You can rotate, move, scale, and another transformations of points
			// Рисует 

		virtual Pen 		getPen(void) {}
		virtual Brush 		getBrush(void) {}
		virtual TextStyle 	getTextStyle(void) {}
		virtual Point_d 	getTextSize(std::wstring text) {}

		virtual void drawPolygon(Polygon_d& points) {}
		virtual void drawPolyline(Polygon_d& points, bool isRoundJoin = false) {}
		virtual void drawLine(Point_d a, Point_d b) {}

		virtual void drawText(Point_d pos, std::wstring text) {}
	protected:
		Pen 		m_pen;
		Brush 		m_brush;
		TextStyle 	m_textStyle;
	};

	class ImageDrawing_aa : public ImageDrawing, public ImageAgg
	{
	public:
		ImageDrawing_aa(Point_i size);
		ImageDrawing_aa(ImageBase* img);
		virtual ~ImageDrawing_aa();

		void setPen(Pen pen);
		void setBrush(Brush brush);
		void setTextStyle(TextStyle style);

		void drawPTo(ImageBase* dst, Polygon_d rect);

		Pen 		getPen(void);
		Brush 		getBrush(void);
		TextStyle 	getTextStyle(void);
		Point_d 	getTextSize(std::wstring text);

		void drawPolygon(Polygon_d& points);
		void drawPolyline(Polygon_d& points, bool isRoundJoin = false);
		void drawLine(Point_d a, Point_d b);

		void drawText(Point_d pos, std::wstring text);

		using ImageAgg::resize;
	private:
		TextStyle 	m_text;
		Pen 		m_pen;
		Brush 		m_brush;
	};

	class ImageDrawing_win : public ImageDrawing 
	{
	public:
		ImageDrawing_win(Point_i size);
		ImageDrawing_win(ImageBase* img);
		virtual ~ImageDrawing_win();

		void setPen(Pen pen);
		void setBrush(Brush brush);
		void setTextStyle(TextStyle style);

		void drawPTo(ImageBase* dst, Polygon_d rect);

		Pen 		getPen(void);
		Brush 		getBrush(void);
		TextStyle 	getTextStyle(void);
		Point_d 	getTextSize(std::wstring text);

		void drawPolygon(Polygon_d& points);
		void drawPolyline(Polygon_d& points, bool isRoundJoin = false);
		void drawLine(Point_d a, Point_d b);

		void drawText(Point_d pos, std::wstring text);
	private:
		HGDIOBJ	m_lastPen;
		HGDIOBJ	m_lastBrush;
		HGDIOBJ	m_lastFont;

		void saveObjects(void);

		TextStyle 	m_text;
		Pen 		m_pen;
		Brush 		m_brush;
	};
	
}

#endif // TWG_IMAGE_DRAWING_INCLUDED