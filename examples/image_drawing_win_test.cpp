#include "twg/twg.h"
#include "twg/image/image_drawing.h"

using namespace twg;

int main() {
	WindowType type(-1,
		Point_i(100, 0),
		Point_i(300, 300), 
		Point_i(-1, -1),
		Point_i(-1, -1),
		L"Test ImageDrawing_win.",
		WINDOW_STANDART);	

	WindowBase wnd(type);

	ImageDrawing_win img(Point_i(300, 300));
	img.clear(White);

	std::vector<Point_d> mas;
	mas.push_back(Point_d(10, 20));
	mas.push_back(Point_d(30, 40));
	mas.push_back(Point_d(50, 100));
	mas.push_back(Point_d(10, 50));
	mas.push_back(Point_d(10, 20));
	Polygon_d poly(mas);

	img.drawLine(Point_i(10, 10), Point_i(30, 30));
	img.drawPolygon(poly);
	img.drawPolyline(poly, false);

	img.setPen(Pen(2, Bitcoin));
	img.setBrush(Brush(Miku));

	poly.move(Point_d(10, 30));

	img.drawLine(Point_i(10, 10)+Point_i(10, 30), Point_i(30, 30)+Point_i(10, 30));
	img.drawPolygon(poly);
	img.drawPolyline(poly, false);

	img.setTextStyle(TextStyle(14, L"Consolas", TEXT_NONE));
	img.setPen(Pen(1, Black));
	img.drawText(Point_i(10, 100), std::wstring(L"Привет, это текст в юникоде.\nThere I can write what I want.\n\tAny symbols!\n\t☺ ☻ ♥ ♦ ♣ ♠ • ◘ ○"));

	img.copyTo(&wnd.canvas, Point_i(0, 0), Point_i(0, 0), img.size());

	wnd.waitForClose();
	return 0;
}