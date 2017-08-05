#include "twg/image/image_drawing.h"

namespace twg
{

//-----------------------------------------------------------------------------
void ImageDrawing_aa::setPen(Pen pen) {

}

//-----------------------------------------------------------------------------
void ImageDrawing_aa::setBrush(Brush brush) {

}

//-----------------------------------------------------------------------------
void ImageDrawing_aa::setTextStyle(int8u width, 
								   std::string name, 
								   TextFlag flags = TEXT_NONE) {
}

//-----------------------------------------------------------------------------
void ImageDrawing_aa::drawTo(ImageWin* dst, 
							 Point_i dstStart, 
							 Point_i srcStart,
							 Point_i srcSize) {

}

//-----------------------------------------------------------------------------
void ImageDrawing_aa::drawTo(ImageBase* dst, Polygon_d rect) {

}

//-----------------------------------------------------------------------------
Pen ImageDrawing_aa::getPen(void) {

}

//-----------------------------------------------------------------------------
Brush ImageDrawing_aa::getBrush(void) {

}

//-----------------------------------------------------------------------------
TextStyle ImageDrawing_aa::getTextStyle(void) {

}

//-----------------------------------------------------------------------------
Point_d ImageDrawing_aa::getTextSize(std::string) {

}

//-----------------------------------------------------------------------------
void ImageDrawing_aa::drawPolygon(Polygon_d points) {

}

//-----------------------------------------------------------------------------
void ImageDrawing_aa::drawPolyline(Polygon_d points, bool isRoundJoin) {

}

//-----------------------------------------------------------------------------
void ImageDrawing_aa::drawLine(Point_d a, Point_d b) {

}

//-----------------------------------------------------------------------------
void ImageDrawing_aa::fillFlood(Point_d pos) {

}

//-----------------------------------------------------------------------------
void ImageDrawing_aa::drawText(Point_d pos, std::string text) {

}

}