#include "twg/image/image_drawing.h"

namespace twg
{

//-----------------------------------------------------------------------------
ImageDrawing_aa::ImageDrawing_aa(ImageBase* img) : ImageDrawing(Point_i(1, 1)) {
	
}

//-----------------------------------------------------------------------------
ImageDrawing_aa::~ImageDrawing_aa() {

}

//-----------------------------------------------------------------------------
void ImageDrawing_aa::setPen(Pen pen) {

}

//-----------------------------------------------------------------------------
void ImageDrawing_aa::setBrush(Brush brush) {

}

//-----------------------------------------------------------------------------
void ImageDrawing_aa::setTextStyle(TextStyle style) {
}

//-----------------------------------------------------------------------------
void ImageDrawing_aa::drawPTo(ImageBase* dst, Polygon_d rect) {

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
Point_d ImageDrawing_aa::getTextSize(std::wstring) {

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
// void ImageDrawing_aa::fillFlood(Point_d pos) {

// }

//-----------------------------------------------------------------------------
void ImageDrawing_aa::drawText(Point_d pos, std::wstring text) {

}

}