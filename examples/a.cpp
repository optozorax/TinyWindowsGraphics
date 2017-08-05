#include <cstdlib>

#include "twg/twg.h"
#include "twg/ctrl/menu.h"
#include "twg/image/image_drawing.h"
#include "twg/window/window_ctrl.h"
#include "twg/window/window_buffer.h"

using namespace twg;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WindowType type(0,
					Point_i(100, 100),
					Point_i(300, 300), 
					Point_i(0, 0),
					Point_i(0, 0),
					"Window #1",
					WINDOW_STANDART);

	WindowBase wnd(type);

	Menu menu(std::string("Файл"), true, Point_i(0, 0), nullptr);

	ImageDrawing_aa img(Point_i(300, 300));

	WindowCtrl wnd2(type);

	WinBufEventsDraw_aa wnd3(type, Point_i(2000, 2000));

	std::system("pause");
	return 0;
}