#include <twg/twg.h>
#include <twg/window/window_events.h>
#include <twg/image/image_drawing.h>

using namespace twg;

class MyWindow : public WindowEvents
{
public:
	MyWindow(WindowType type) : 
		WindowEvents(type), 
		img(type.size),
		scale(1),
		pos_d(type.size / 2.0),
		isMoving(false) {}

	bool onMouse(Point_i pos, MouseType type);
	bool onResize(Rect rect, SizingType type);

	void redraw(void);

	ImageDrawing_aa img;
	Point_d pos_d;
	double scale;

	Point_i lastPos;
	bool isMoving;
};

//=============================================================================
//=============================================================================
//=============================================================================

//-----------------------------------------------------------------------------
bool MyWindow::onMouse(Point_i pos, MouseType type) {
	if (type == MOUSE_L_DOWN) {
		isMoving = true;
		lastPos = pos;
		return true;
	}
	if (type == MOUSE_L_UP) {
		isMoving = false;
		return true;
	}

	if (type == MOUSE_WHEEL_DOWN && scale > 0.1) {
		Point_d locPos = (pos - pos_d)/scale;
		scale *= 0.9;
		pos_d = pos - locPos * scale;
		redraw();
		return true;
	}
	if (type == MOUSE_WHEEL_UP) {
		Point_d locPos = (pos - pos_d)/scale;
		scale *= 1.111111111111111111;
		pos_d = pos - locPos * scale;
		redraw();
		return true;
	}

	if (type == MOUSE_MOVE && isMoving) {
		pos_d += (pos-lastPos);
		lastPos = pos;
		redraw();
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
bool MyWindow::onResize(Rect rect, SizingType type) {
	if (rect.x() > img.width() || rect.y() > img.height())
		img.resize(Point_i(rect.x() + 100, rect.y() + 100));
	redraw();

	return true;
}

//-----------------------------------------------------------------------------
void MyWindow::redraw(void) {
	img.clear(White);
	img.setPen(Pen(10*scale, Black));

	Polygon_d axis;
	std::vector<Point_d>& arr = axis.array;
	arr.push_back(Point_d(-1, 0));
	arr.push_back(Point_d(1, 0));
	arr.push_back(Point_d(0.1, 0).rotate(deg2rad(-135)) + Point_d(1, 0));
	arr.push_back(Point_d(0.1, 0).rotate(deg2rad(135)) + Point_d(1, 0));

	arr.push_back(Point_d(0, 1));
	arr.push_back(Point_d(0, -1));
	arr.push_back(Point_d(0, -0.1).rotate(deg2rad(-135)) + Point_d(0, -1));
	arr.push_back(Point_d(0, -0.1).rotate(deg2rad(135)) + Point_d(0, -1));

	axis.scale(Point_d(100, 100));
	axis.scale(Point_d(scale, scale));
	axis.move(pos_d);

	img.drawLine(arr[0], arr[1]);
	img.drawLine(arr[1], arr[2]);
	img.drawLine(arr[1], arr[3]);
	img.drawLine(arr[4], arr[5]);
	img.drawLine(arr[5], arr[6]);
	img.drawLine(arr[5], arr[7]);

	img.copyTo(&canvas, Point_i(0, 0), Point_i(0, 0), img.size());
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// int main() {
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WindowType type(stdIcon,
		Point_i(100, 100),
		Point_i(300, 300),
		stdMin, stdMax,
		L"You can move and scaling image",
		WINDOW_STANDART);
	MyWindow wnd(type);

	wnd.redraw();

	wnd.waitForClose();
}