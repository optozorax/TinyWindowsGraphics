#include "twg/twg.h"
#include "twg/image/image_drawing.h"

using namespace twg;

template <class Image>
WindowBase* createWindow(std::wstring caption, Point_i position) {
	WindowType type(-1,
		position,
		Point_i(350, 350), 
		Point_i(350, 350),
		Point_i(350, 350),
		caption,
		WINDOW_STANDART);	

	WindowBase* wnd = new WindowBase(type);

	Image img(Point_i(350, 350));
	img.clear(White);

	const double scale = 2.0;
	Image img2(Point_i(100, 100)*scale);
	img2.clear(Transparent);
	img2.setPen(Pen(1, Bitcoin));
	Point_d a1 = Point_d(0, 0);
	Point_d a2 = Point_d(int32u(100*scale)-1, 0);
	Point_d a3 = Point_d(0, int32u(100*scale)-1);
	img2.drawLine(a1, a2);
	img2.drawLine(a2, a2+a3);
	img2.drawLine(a2+a3, a3);
	img2.drawLine(a3, a1);
	img2.setPen(Pen(1, Miku));

	img.copyTo(&wnd->canvas, Point_i(0, 0), Point_i(0, 0), img.size());


	//-------------------------------------------------------------------------
	// Draw in first image
	Polygon_d poly;
	poly.array.push_back(Point_d(10, 20));
	poly.array.push_back(Point_d(30, 40));
	poly.array.push_back(Point_d(50, 100));
	poly.array.push_back(Point_d(10, 50));
	poly.array.push_back(Point_d(10, 20));

	img.drawLine(Point_i(10, 10), Point_i(30, 30));
	img.drawPolygon(poly);
	img.drawPolyline(poly, false);

	img.copyTo(&wnd->canvas, Point_i(0, 0), Point_i(0, 0), img.size());

	img.setPen(Pen(2, setAlpha(Bitcoin, 128)));
	img.setBrush(Brush(setAlpha(Miku, 128)));

	poly.move(Point_d(10, 0));

	img.drawLine(Point_i(10, 10)+Point_i(10, 0), Point_i(30, 30)+Point_i(10, 0));
	img.drawPolygon(poly);
	img.drawPolyline(poly, false);

	poly.move(-poly.array[0]);
	poly.scale(Point_d(2, 2));
	poly.move(Point_d(200, 150));

	img.setPen(Pen(2, setAlpha(SeaGreen, 128)));
	img.setBrush(Brush(setAlpha(Dollar, 50)));

	img.drawPolygon(poly);
	img.drawPolyline(poly, false);

	img.setTextStyle(TextStyle(14, L"Consolas", TEXT_NONE));
	img.setPen(Pen(1, Black));
	img.drawText(Point_i(10, 100), std::wstring(L"Привет, это текст в Юникоде.\nThere I can write what I want.\n\tAny symbols!\n\t☺ ☻ ♥ ♦ ♣ ♠ • ◘ ○\nおはよう µ² Unicode ┌──┘"));

	img.copyTo(&wnd->canvas, Point_i(0, 0), Point_i(0, 0), img.size());


	//-------------------------------------------------------------------------
	// Draw in second image

	img2.setTextStyle(TextStyle(12*scale, L"Lucida Sans Unicode", TEXT_NONE));
	img2.drawText(Point_i(2, 2)*scale, L"Any symbols!\n\t☺ ☻ ♥ ♦ ♣ ♠ • ◘ ○\nおはよう µ² Unicode ┌──┘\næ¾¶Δ۝۩ٮ╛▓ﺕ");

	// Crutch for ImageDrawing_win - because Windows drawing with alpha channel == 0. It works bad for black color and pixels which early be transparent.
	PointIterator<Point_i> i(Point_i(0, 0), img2.size()-Point_i(1, 1));
	for (i.onStart(); !i.isEnd(); ++i) {
		Color& pix = img2[*i];
		if (pix != Transparent && getAlpha(pix) == 0)
			pix = setAlpha(pix, 255);
	}

	img.copyTo(&wnd->canvas, Point_i(0, 0), Point_i(0, 0), img.size());


	//-------------------------------------------------------------------------
	// Draw second image in first image
	poly.array.erase(poly.array.begin(), poly.array.end());
	poly.array.push_back(Point_d(0, 0));
	poly.array.push_back(Point_d(150, 0));
	poly.array.push_back(Point_d(100, 100));
	poly.array.push_back(Point_d(0, 150));
	poly.move(Point_d(180, 135));
	img.drawPTo(&img2, poly);

	img.copyTo(&wnd->canvas, Point_i(0, 0), Point_i(0, 0), img.size());


	poly.array.erase(poly.array.begin(), poly.array.end());
	poly.array.push_back(Point_d(0, 0));
	poly.array.push_back(Point_d(100, 0));
	poly.array.push_back(Point_d(100, 100));
	poly.array.push_back(Point_d(0, 100));
	poly.rotate(pi/3, Point_d(50, 50));
	poly.move(Point_d(30, 190));
	img.drawPTo(&img2, poly);

	img.copyTo(&wnd->canvas, Point_i(0, 0), Point_i(0, 0), img.size());


	img2.drawTo(&img, Point_i(100, 0), Point_i(0, 0), Point_i(img2.width(), img2.height()/scale));

	//-------------------------------------------------------------------------
	// Draw it on screen
	img.copyTo(&wnd->canvas, Point_i(0, 0), Point_i(0, 0), img.size());

	return wnd;
}

int main() {
	WindowBase* wnd1 = createWindow<ImageDrawing_aa>(L"ImageDrawing_aa", Point_i(100, 100));
	WindowBase* wnd2 = createWindow<ImageDrawing_win>(L"ImageDrawing_win", Point_i(450, 100));

	wnd1->waitForClose();
	wnd2->waitForClose();
	return 0;
}