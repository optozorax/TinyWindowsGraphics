#include <windows.h>
#include "twg/twg.h"
#include "twg/window/window_buffer.h"

using namespace twg;

// Функции для получения координат прямоугольников по краям экрана.
enum RectangleType : int32u;
void 		  getRectangle(RectangleType type, 
						   Point_i& pos, 
						   Point_i& size, 
						   Point_i windowSize, 
						   Point_i rectSize = Point_i(10, 10));
RectangleType getRectangleType(Point_i pos, 
							   Point_i windowSize, 
							   Point_i rectSize = Point_i(10, 10));

//-----------------------------------------------------------------------------

enum RectangleType : int32u
{
	RECTANGLE_TOP,
	RECTANGLE_BOTTOM,
	RECTANGLE_LEFT,
	RECTANGLE_RIGHT,

	RECTANGLE_TOP_LEFT,
	RECTANGLE_TOP_RIGHT,
	RECTANGLE_BOTTOM_LEFT,
	RECTANGLE_BOTTOM_RIGHT,

	RECTANGLE_NOWHERE
};

//-----------------------------------------------------------------------------

class MyWindow;
void drawRect(ImageBase& img, Point_i a, Point_i b, Color clr);
void redrawWindow(MyWindow& wnd, Point_i size);

//-----------------------------------------------------------------------------

class MyWindow : public WindowBuffer<WindowEvents, ImageBase>
{
public:
	MyWindow(WindowType type) : WindowBuffer(type, Point_i(1000, 1000)) {}
	~MyWindow() {}

	bool onKeyboard(KeyType key, bool isDown);

	bool onResize(Rect rect, SizingType type);
	bool onMessage(int32u messageNo, void* data);
};

//=============================================================================
//=============================================================================
//=============================================================================

void getRectangle(RectangleType type, Point_i& pos, Point_i& size, Point_i windowSize, Point_i rectSize) {
	Point_i& rect = rectSize;
	Point_i& wnd = windowSize;
	switch (type) {
		case RECTANGLE_TOP:
			pos = Point_i(rect.x, 0);
			size = Point_i(wnd.x-2*rect.x, rect.y);
			break;
		case RECTANGLE_BOTTOM:
			pos = Point_i(rect.x, wnd.y-rect.y);
			size = Point_i(wnd.x-2*rect.x, rect.y);
			break;
		case RECTANGLE_LEFT:
			pos = Point_i(0, rect.y);
			size = Point_i(rect.x, wnd.y-2*rect.y);
			break;
		case RECTANGLE_RIGHT:
			pos = Point_i(wnd.x-rect.x, rect.y);
			size = Point_i(rect.x, wnd.y-2*rect.y);
			break;

		case RECTANGLE_TOP_LEFT:
			pos = Point_i(0, 0);
			size = rect;
			break;
		case RECTANGLE_BOTTOM_RIGHT:
			pos = wnd-rect;
			size = rect;
			break;
		case RECTANGLE_TOP_RIGHT:
			pos = Point_i(wnd.x-rect.x, 0);
			size = rect;
			break;
		case RECTANGLE_BOTTOM_LEFT:
			pos = Point_i(0, wnd.y-rect.y);
			size = rect;
			break;
	}
}

RectangleType getRectangleType(Point_i pos, Point_i windowSize, Point_i rectSize) {
	for (int32u i = 0; i <= 8; ++i) {
		Point_i a, b;
		getRectangle(i, a, b, windowSize, rectSize);
		if (pos.inRectangle(a, a+b)) return i;
	}
	return RECTANGLE_NOWHERE;
}

//-----------------------------------------------------------------------------

bool MyWindow::onKeyboard(KeyType key, bool isDown) {
	if (key == KEY_ESCAPE)
		SendMessage(m_hwnd, WM_CLOSE, 0, 0);
}

bool MyWindow::onResize(Rect rect, SizingType type) {
	Point_i newSize(rect.x(), rect.y()); 
	Point_i pos(rect.ax, rect.ay);
	redrawWindow(*this, newSize);
}

bool MyWindow::onMessage(int32u messageNo, void* data) {
	if (messageNo == WINDOWS_MESSAGE) {
		onMessageStruct* msg = data;
		switch (msg->msg) {
			case WM_NCHITTEST: {
				Point_i pos(LOWORD(msg->lParam), HIWORD(msg->lParam));
				pos -= getPos();
				switch (getRectangleType(pos, getClientSize())) {
					case RECTANGLE_TOP:
						msg->lResult = HTTOP; 
						break;
					case RECTANGLE_BOTTOM:
						msg->lResult = HTBOTTOM; 
						break;
					case RECTANGLE_LEFT:
						msg->lResult = HTLEFT; 
						break;
					case RECTANGLE_RIGHT:
						msg->lResult = HTRIGHT; 
						break;

					case RECTANGLE_TOP_LEFT:
						msg->lResult = HTTOPLEFT; 
						break;
					case RECTANGLE_TOP_RIGHT:
						msg->lResult = HTTOPRIGHT; 
						break;
					case RECTANGLE_BOTTOM_LEFT:
						msg->lResult = HTBOTTOMLEFT; 
						break;
					case RECTANGLE_BOTTOM_RIGHT:
						msg->lResult = HTBOTTOMRIGHT; 
						break;
					case RECTANGLE_NOWHERE:
						msg->lResult = HTCAPTION; 
						break;
				}

				if (msg->lResult != 0) return true;
			} break;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------

void drawRect(ImageBase& img, Point_i a, Point_i b, Color clr) {
	PointIterator<Point_i> i(a, a+b-Point_i(1, 1));
	for (i.onStart(); !i.isEnd(); ++i)
		img[*i] = computeOverlay(clr, img[*i]);
}

void redrawWindow(MyWindow& wnd, Point_i size) {
	// Ресайзим буфер, если окно размером больше него.
	if (size.x > wnd.buffer.width() || size.y > wnd.buffer.height()) {
		if (size.x > wnd.buffer.width())
			wnd.buffer.resize(Point_i(size.x + 100, wnd.buffer.height()));
		if (size.y > wnd.buffer.height())
			wnd.buffer.resize(Point_i(wnd.buffer.width(), size.y + 100));
	}

	Point_i res(10, 10);
	Color cornerColor = setAlpha(Black, 128);
	Color borderColor = setAlpha(Black, 192);

	wnd.buffer.clear(Transparent);

	// Рисование границ окна
	for (int i = 0; i < 4; ++i) {
		Point_i pos;
		Point_i width;
		getRectangle(i, pos, width, size);
		drawRect(wnd.buffer, pos, width, borderColor);
	}

	// Рисование углов окна
	for (int i = 4; i < 8; ++i) {
		Point_i pos;
		Point_i width;
		getRectangle(i, pos, width, size);
		drawRect(wnd.buffer, pos, width, cornerColor);
	}

	// Рисование того, что внутри окна
	int32u width = size.x-res.x*2;
	int32u height = size.y-res.y*2;
	for (int i = 0; i < width; ++i) {
		Color clr = setAlpha(getRainbow(double(i)/width), 200);
		for (int j = 0; j < height; ++j) {
			wnd.buffer[res+Point_i(i,j)] = clr;
		}
	}

	wnd.redraw();
}

//-----------------------------------------------------------------------------

int main() {
	WindowType type(-1, Point_i(100, 100), Point_i(100, 100), Point_i(20, 20), Point_i(5000, 5000), L"WINDOW_IMAGE test.", WINDOW_IMAGE32 | WINDOW_ON_TOP);
	MyWindow wnd(type);	
	redrawWindow(wnd, wnd.getClientSize());
	wnd.waitForClose();
}