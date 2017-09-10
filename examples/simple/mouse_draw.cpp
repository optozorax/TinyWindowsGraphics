#include <twg/twg.h>
#include <twg/window/window_events.h>

using namespace twg;

class MyWindow : public WindowEvents
{
public:
	MyWindow(WindowType type) : WindowEvents(type), img(Point_i(500, 500)) {
		img.clear(White);
	}

	bool onMouse(Point_i pos, MouseType type);

	ImageBase img;
};

//=============================================================================
//=============================================================================
//=============================================================================

//-----------------------------------------------------------------------------
bool MyWindow::onMouse(Point_i pos, MouseType type) {
	static bool isDrawing = false;
	static Point_i lastPos;
	if (type == MOUSE_L_DOWN) {
		lastPos = pos;
		isDrawing = true;

		return true;
	}
	if (type == MOUSE_L_UP) {
		isDrawing = false;

		return true;
	}
	if (type == MOUSE_MOVE && isDrawing) {
		// Рисуем линию
		Point_i diff = pos-lastPos;
		for (int i = 0; i < diff.getLength(); ++i)
			img[lastPos + diff*i/diff.getLength()] = Black;
		lastPos = pos;

		// Рисуем картинку на экран
		img.drawTo(&canvas, Point_i(0, 0), Point_i(0, 0), img.size());

		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WindowType type(stdIcon,
		Point_i(100, 100),
		Point_i(300, 300),
		Point_i(100, 100), Point_i(500, 500), 
		L"You can draw in window.",
		WINDOW_STANDART);

	MyWindow wnd(type);

	wnd.waitForClose();
}