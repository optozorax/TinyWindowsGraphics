#include <cstdlib>
#include <string>
#include "twg/twg.h"

using namespace twg;

int main() {
	WindowType type(-1,
					Point_i(100, 0),
					Point_i(300, 70), 
					Point_i(-1, -1),
					Point_i(-1, -1),
					"",
					WINDOW_STANDART);	

	type.style = WINDOW_STANDART;
	type.caption = "Window standart";
	type.pos += Point_i(0, 100);
	WindowBase wnd1(type);

	type.style = WINDOW_TOOL;
	type.caption = "Window tool";
	type.pos += Point_i(0, 100);
	WindowBase wnd2(type);
	
	type.style = WINDOW_CAPTION;
	type.caption = "Window caption";
	type.pos += Point_i(0, 100);
	WindowBase wnd3(type);
	
	type.style = WINDOW_POPUP;
	type.caption = "Window popup";
	type.maxSize = Point_i(400, 100);
	type.minSize = Point_i(0, 0);
	type.pos += Point_i(0, 100);
	WindowBase wnd4(type);
	
	type.style = WINDOW_NO_BORDER;
	type.caption = "Window no border";
	type.maxSize = Point_i(-1, -1);
	type.minSize = Point_i(-1, -1);
	type.pos += Point_i(0, 100);
	WindowBase wnd5(type);

	std::system("pause");

	Point_i center = wnd1.getPos();
	Point_i lastPos = wnd2.getPos();
	int j = 0, k = 0;
	for (int i = 0; i <= 360; i += 3) {
		j++; if (j > 5) { j = 0; k++; }

		Point_i pos = lastPos;
		pos.rotate(i/180.0 * pi, center);
		wnd2.setPos(pos);
		wnd3.setCaption(std::to_string(i) + " degrees of rotate");
		wnd4.setWindowSize(Point_i(i/360.0 * 300, 70));
		wnd5.setStyle(k % WINDOW_MAX_TYPE);

		sleep(30);
	}

	std::system("pause");

	return 0;
}