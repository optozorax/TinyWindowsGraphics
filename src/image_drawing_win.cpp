#include <string>
#include "twg/image/image_drawing.h"

namespace twg
{

//-----------------------------------------------------------------------------
ImageDrawing_win::ImageDrawing_win(Point_i size) : ImageBase(size), ImageDrawing(size) {
	saveObjects();
}

//-----------------------------------------------------------------------------
ImageDrawing_win::ImageDrawing_win(ImageBase* img) : ImageBase(Point_i(1, 1)), ImageDrawing(Point_i(1, 1)) {
	assign(img);
	saveObjects();
}

//-----------------------------------------------------------------------------
ImageDrawing_win::~ImageDrawing_win() {
	DeleteObject(SelectObject(getHdc(), m_lastPen));
	DeleteObject(SelectObject(getHdc(), m_lastBrush));
	DeleteObject(SelectObject(getHdc(), m_lastFont));
}

//-----------------------------------------------------------------------------
void ImageDrawing_win::saveObjects(void) {
	m_lastPen = SelectObject(getHdc(), CreatePen(PS_NULL, 0, 0));
	m_lastBrush = SelectObject(getHdc(), GetSysColorBrush(NULL_BRUSH));
	LOGFONT font = {};
	font.lfHeight 			= 16;
	font.lfWeight 			= 100;
	font.lfCharSet 			= RUSSIAN_CHARSET;
	font.lfOutPrecision 	= OUT_STROKE_PRECIS;
	font.lfClipPrecision 	= CLIP_DEFAULT_PRECIS;
	font.lfQuality 			= ANTIALIASED_QUALITY;
	std::wstring name(L"Consolas");
	for (int i = 0; i < name.size(); i++)
		font.lfFaceName[i] = name[i];
	for (int i = name.size(); i < 32; i++)
		font.lfFaceName[i] = 0;
	HFONT hfont = CreateFontIndirect(&font);
	m_lastFont = SelectObject(getHdc(), hfont);

	setPen(Pen(1, Black));
	setBrush(Brush(White));
	setTextStyle(TextStyle(12, std::wstring(L"Consolas"), TEXT_NONE));
}

//-----------------------------------------------------------------------------
void ImageDrawing_win::setPen(Pen pen) {
	if (pen.clr == Transparent)
		DeleteObject(SelectObject(getHdc(), CreatePen(PS_NULL, 0, 0)));
	else 
		DeleteObject(SelectObject(getHdc(), CreatePen(PS_SOLID, (int) pen.thick, RGB(getRed(pen.clr), getGreen(pen.clr), getBlue(pen.clr)))));

	SetTextColor(getHdc(), RGB(getRed(pen.clr), getGreen(pen.clr), getBlue(pen.clr)));

	m_pen = pen;
}

//-----------------------------------------------------------------------------
void ImageDrawing_win::setBrush(Brush brush) {
	SetBkMode(getHdc(), OPAQUE);
	if (brush.clr == Transparent)
		DeleteObject(SelectObject(getHdc(), GetSysColorBrush(NULL_BRUSH)));
	else 
		DeleteObject(SelectObject(getHdc(), CreateSolidBrush(RGB(getRed(brush.clr), getGreen(brush.clr), getBlue(brush.clr)))));

	m_brush = brush;
}

//-----------------------------------------------------------------------------
void ImageDrawing_win::setTextStyle(TextStyle style) {
	LOGFONT font = {};
	font.lfHeight 			= style.width; // Отрицательно - в пикселях, положительно - в стандартных единицах измерения.
	font.lfWeight 			= (style.flags & TEXT_BOLD) ? 1000 : 0;
	font.lfItalic 			= style.flags & TEXT_ITALIC;
	font.lfUnderline 		= style.flags & TEXT_UNDERLINE;
	font.lfStrikeOut 		= style.flags & TEXT_STRIKE;
	font.lfCharSet 			= DEFAULT_CHARSET; 
	font.lfOutPrecision 	= OUT_STROKE_PRECIS;
	font.lfClipPrecision 	= CLIP_DEFAULT_PRECIS;
	font.lfQuality 			= ANTIALIASED_QUALITY;

	/* Название шрифта. */
	for (int i = 0; i < style.name.size(); i++)
		font.lfFaceName[i] = style.name[i];
	for (int i = style.name.size(); i < 32; i++)
		font.lfFaceName[i] = 0;

	/* Применение шрифта к дескриптору окна. */
	HFONT hfont = CreateFontIndirect(&font);
	DeleteObject(SelectObject(m_hdc, hfont));

	m_text = style;
}

//-----------------------------------------------------------------------------
void ImageDrawing_win::drawPTo(ImageBase* dst, Polygon_d rect) {
	//ImageDrawing_aa* img = new ImageDrawing_aa(this);
	ImageDrawing_aa img(this);
	img.drawPTo(dst, rect);
}

//-----------------------------------------------------------------------------
Pen ImageDrawing_win::getPen(void) {
	return m_pen;
}

//-----------------------------------------------------------------------------
Brush ImageDrawing_win::getBrush(void) {
	return m_brush;
}

//-----------------------------------------------------------------------------
TextStyle ImageDrawing_win::getTextStyle(void) {
	return m_text;
}

//-----------------------------------------------------------------------------
Point_d ImageDrawing_win::getTextSize(std::wstring text) {
	while (text.find(L"\t") != std::string::npos)
		text.replace(text.find(L"\t"), 1, L"        ");
	
	SIZE *sz = new SIZE;
	std::wstring first;
	Point_i x(0, 0);

	do {
		first = text.substr(0, text.find(L"\n"));
		GetTextExtentPoint32(getHdc(), first.c_str(), first.size(), sz);
		x = Point_i(TWG_max(x.x, sz->cx), TWG_max(x.y, sz->cy));
		text.erase(0, first.size()+1);
	} while (text.size() != 0);

	delete sz;
	return Point_d(x);
}

//-----------------------------------------------------------------------------
void ImageDrawing_win::drawPolygon(Polygon_d& points) {
	Pen pen = getPen();
	setPen(Pen(0, Transparent));
	std::vector<Point_d> ps = points.array;
	POINT *mas1 = new POINT[ps.size()];
	for (int i = 0; i < ps.size(); i++) {
		mas1[i] = {int32(ps[i].x), int32(ps[i].y)};
	}
	Polygon(m_hdc, mas1, ps.size());
	delete mas1;
	setPen(pen);
}

//-----------------------------------------------------------------------------
void ImageDrawing_win::drawPolyline(Polygon_d& points, bool isRoundJoin) {
	std::vector<Point_d> ps = points.array;
	POINT *mas1 = new POINT[ps.size()];
	for (int i = 0; i < ps.size(); i++) {
		mas1[i] = {(int32)(ps[i].x), (int32)(ps[i].y)};
	}
	Polyline(m_hdc, mas1, ps.size());
	delete mas1;
}

//-----------------------------------------------------------------------------
void ImageDrawing_win::drawLine(Point_d a, Point_d b) {
	MoveToEx(getHdc(), a.x, a.y, NULL);
	LineTo(getHdc(), b.x, b.y);
}

//-----------------------------------------------------------------------------
void ImageDrawing_win::drawText(Point_d pos, std::wstring text) {
	SetBkMode(m_hdc, TRANSPARENT);
	while (text.find(L"\t") != std::string::npos)
		text.replace(text.find(L"\t"), 1, L"        ");

	std::wstring first;
	int32u y = 0;
	do {
		first = text.substr(0, text.find(L"\n"));
		TextOut(getHdc(), pos.x, pos.y + y, first.c_str(), first.size()); 
		y = y + getTextSize(first).y;
		text.erase(0, first.size()+1);
	} while (text.size() != 0);
	SetBkMode(m_hdc, OPAQUE);
}

}