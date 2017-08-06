#include <cstdlib>
#include "twg/twg.h"

using namespace twg;

int main() {
	ImageWin screen;
	screen.assignScreen();

	ImageBase img(screen.size());
	screen.copyTo(&img, Point_i(0, 0), Point_i(0, 0), screen.size());

	WindowType type(-1,
					Point_i(100, 100),
					Point_i(300, 300), 
					Point_i(300, 300),
					Point_i(300, 300),
					L"Window",
					WINDOW_STANDART);	

	WindowBase wnd1(type);

	img.copyTo(&wnd1.canvas, Point_i(0, 0), Point_i(0, 0), img.size());
	std::system("pause");

	messageBox(std::wstring(L"Text"), std::wstring(L"Caption"), MESSAGE_YES_NO_CANCEL | MESSAGE_ICON_QUESTION | MESSAGE_ALIGN_RIGHT_ALL | MESSAGE_STYLE_TOPMOST);

	saveToBmp(&img, L"screen.bmp");

	img.resize(Point_i(300, 300));
	img.clear(Miku);
	saveToBmp(&img, L"img1.bmp");

	img.clear(setBlue(Miku, 0));
	saveToBmp(&img, L"img2.bmp");

	img.copyTo(&wnd1.canvas, Point_i(0, 0), Point_i(0, 0), img.size());
	std::system("pause");
	return 0;
}