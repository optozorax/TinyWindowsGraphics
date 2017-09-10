#include <twg/twg.h>

using namespace twg;

class MyWindow : public WindowBase
{
public:
	MyWindow(WindowType type) : WindowBase(type) {}

private:
	LRESULT wndProcNext(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

//=============================================================================
//=============================================================================
//=============================================================================

//-----------------------------------------------------------------------------
LRESULT MyWindow::wndProcNext(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_MOVING: {
			RECT* rect = (RECT*)lParam;
			HDC hdc = canvas.getHdc();
			PatBlt(hdc, 0, 0, 300, 300, WHITENESS);
			Ellipse(hdc, 250 - rect->left, 250 - rect->top, 270 - rect->left, 270 - rect->top);

			return TRUE;
			} break;

		case WM_NCHITTEST:
			return HTCAPTION;

		case WM_ERASEBKGND:
			return TRUE;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

int main() {
	WindowType type(twg::stdIcon,
		twg::Point_i(100, 100),
		twg::Point_i(300, 300),
		twg::stdMin,
		twg::stdMax,
		L"Hello World",
		twg::WINDOW_STANDART);

	MyWindow wnd(type);

	HWND hwnd = wnd.getHwnd();
	MoveWindow(hwnd, 120, 120, 300, 300, TRUE);

	wnd.waitForClose();
}