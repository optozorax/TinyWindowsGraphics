#include "twg/image/image_drawing.h"

namespace twg
{

//-----------------------------------------------------------------------------
void ImageDrawing_win::setPen(Pen pen) {

}

//-----------------------------------------------------------------------------
void ImageDrawing_win::setBrush(Brush brush) {

}

//-----------------------------------------------------------------------------
void ImageDrawing_win::setTextStyle(int8u width, 
									std::wstring name, 
									TextFlag flags = TEXT_NONE) {
}

//-----------------------------------------------------------------------------
void ImageDrawing_win::drawTo(ImageWin* dst, 
							  Point_i dstStart, 
							  Point_i srcStart,
							  Point_i srcSize) {

}

//-----------------------------------------------------------------------------
void ImageDrawing_win::drawTo(ImageBase* dst, Polygon_d rect) {

}

//-----------------------------------------------------------------------------
Pen ImageDrawing_win::getPen(void) {

}

//-----------------------------------------------------------------------------
Brush ImageDrawing_win::getBrush(void) {

}

//-----------------------------------------------------------------------------
TextStyle ImageDrawing_win::getTextStyle(void) {

}

//-----------------------------------------------------------------------------
Point_d ImageDrawing_win::getTextSize(std::wstring) {

}

//-----------------------------------------------------------------------------
void ImageDrawing_win::drawPolygon(Polygon_d points) {

}

//-----------------------------------------------------------------------------
void ImageDrawing_win::drawPolyline(Polygon_d points, bool isRoundJoin) {

}

//-----------------------------------------------------------------------------
void ImageDrawing_win::drawLine(Point_d a, Point_d b) {

}

//-----------------------------------------------------------------------------
void ImageDrawing_win::fillFlood(Point_d pos) {

}

//-----------------------------------------------------------------------------
void ImageDrawing_win::drawText(Point_d pos, std::wstring text) {

}

}