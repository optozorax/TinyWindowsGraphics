#include <twg/twg.h>

int main() {
	twg::WindowType type(twg::stdIcon,
		twg::Point_i(100, 100),
		twg::Point_i(300, 300),
		twg::stdMin,
		twg::stdMax,
		L"Hello World",
		twg::WINDOW_STANDART);

	twg::WindowBase wnd(type);

	wnd.waitForClose();
}