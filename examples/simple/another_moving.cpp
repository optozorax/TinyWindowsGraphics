#include <twg/twg.h>
#include <twg/window/window_events.h>

using namespace twg;

class MyWindow : public WindowEvents
{
public:
	MyWindow(WindowType type) : WindowEvents(type) {}

	bool onMove(Point_i newPos);

	MyWindow* another;
	Point_i lastPos;
	bool notMove;
};

//=============================================================================
//=============================================================================
//=============================================================================

//-----------------------------------------------------------------------------
bool MyWindow::onMove(Point_i newPos) {
	if (!notMove) {
		Point_i diff = newPos - lastPos;
		setPos(lastPos);
		another->notMove = true;
		another->setPos(another->lastPos + diff);
		another->lastPos = another->lastPos + diff;
	}
	notMove = false;

	return true;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WindowType type(stdIcon,
		Point_i(100, 100),
		Point_i(100, 100),
		Point_i(100, 100), Point_i(100, 100), 
		L"Move another window.",
		WINDOW_TOOL);

	MyWindow wnd1(type);
	wnd1.lastPos = type.pos;

	type.pos += Point_i(100, 100);

	MyWindow wnd2(type);
	wnd2.lastPos = type.pos;

	wnd1.notMove = false;
	wnd2.notMove = false;

	wnd1.another = &wnd2;
	wnd2.another = &wnd1;

	while (!wnd1.isClosed() && !wnd2.isClosed()) {
		sleep(10);
	}
}