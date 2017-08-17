#include <string>
#include "twg/twg.h"

using namespace twg;

int main() {
	WindowType type(-1,
					Point_i(100, 0),
					Point_i(300, 50), 
					Point_i(-1, -1),
					Point_i(-1, -1),
					L"",
					WINDOW_STANDART);	

	type.style = WINDOW_STANDART;
	type.caption = L"Window standart";
	type.pos += Point_i(0, 70);
	WindowBase wnd1(type);

	type.style = WINDOW_TOOL;
	type.caption = L"Window tool"; // おはよう µ² Unicode ┌──┘
	type.pos += Point_i(0, 70);
	WindowBase wnd2(type);
	
	type.style = WINDOW_CAPTION;
	type.caption = L"Window caption";
	type.pos += Point_i(0, 70);
	WindowBase wnd3(type);
	
	type.style = WINDOW_POPUP;
	type.caption = L"Window popup";
	type.maxSize = Point_i(400, 100);
	type.minSize = Point_i(0, 0);
	type.pos += Point_i(0, 70);
	WindowBase wnd4(type);
	
	type.style = WINDOW_NO_BORDER;
	type.caption = L"Window no border";
	type.maxSize = Point_i(-1, -1);
	type.minSize = Point_i(-1, -1);
	type.pos += Point_i(0, 70);
	WindowBase wnd5(type);

	messageBox(std::wstring(L"Сейчас был тест создания окон разного стиля.\nВы можете наблюдать стили окон соответственно:\n\tWINDOW_STANDART\n\tWINDOW_TOOL\n\tWINDOW_CAPTION\n\tWINDOW_POPUP\n\tWINDOW_NO_BORDER\n\nОкна закроются автоматически по завершению тестов.\n\nДалее будет тест измененеия параметров окна во время работы, такие как:\n\tДвижение окна\n\tИзменение заголовка\n\tИзменение размеров\n\tИзменение стиля"), std::wstring(L"Test #1"), MESSAGE_OK | MESSAGE_ICON_INFORMATION | MESSAGE_STYLE_TOPMOST);

	Point_i center = wnd1.getPos();
	Point_i lastPos = wnd2.getPos();
	int j = 0, k = 0;
	for (int i = 0; i <= 360; i += 3) {
		j++; if (j > 5) { j = 0; k++; }

		Point_i pos = lastPos;
		pos.rotate(i/180.0 * pi, center);
		wnd2.setPos(pos);
		wnd3.setCaption(str2wstr(std::to_string(i)) + L" degrees of rotate");
		wnd4.setWindowSize(Point_i(i/360.0 * 300, 50));
		wnd5.setStyle(k % WINDOW_MAX_TYPE);

		sleep(30);
	}

	messageBox(std::wstring(L"Все тесты окончены"), std::wstring(L"Test #2"), MESSAGE_OK | MESSAGE_ICON_INFORMATION | MESSAGE_STYLE_TOPMOST);

	return 0;
}