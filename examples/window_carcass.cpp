#include <map>
#include "twg/twg.h"
#include "twg/window/window_events.h"

using namespace twg;

class CtrlWindow;
enum CtrlWindowMessages : int32u;
struct WhenResize;
struct WhenMove;
class WindowCarcass;

//-----------------------------------------------------------------------------
class CtrlWindow : public WindowEvents
{
public:
	CtrlWindow(WindowType type, EventsBase* parent);
	bool onResize(Rect* rect, SizingType type);
	bool onMove(Point_i newPos);
};

//-----------------------------------------------------------------------------
enum WindowCarcassMessages : int32u
{
	WINDOW_CREATE,
	WINDOW_RESIZE,
	WINDOW_MOVE
};

//-----------------------------------------------------------------------------
struct WhenResize {
	CtrlWindow* pointer;
	Rect* rect;
	SizingType type;
};

//-----------------------------------------------------------------------------
struct WhenMove {
	CtrlWindow* pointer;
	Point_i newPos;
};

//-----------------------------------------------------------------------------
// Представляет собой хранилище контролов-окон
class WindowCarcass : public EventsHandler
{
public:
	WindowCarcass() : EventsHandler(nullptr) {}

	void* sendMessageUp(int32u messageNo, void* data);

	std::vector<WindowEvents*> 			array;
private:
	std::map<WindowEvents*, Point_d> 	m_positions;
	std::map<WindowEvents*, Point_d> 	m_sizes;
	Point_d 							min;
	Point_d 							max;
};

//=============================================================================
//=============================================================================
//=============================================================================

//-----------------------------------------------------------------------------
CtrlWindow::CtrlWindow(WindowType type, EventsBase* parent) : WindowEvents(type, parent) {
	sendMessageUp(WINDOW_CREATE, this);
}

//-----------------------------------------------------------------------------
bool CtrlWindow::onResize(Rect* rect, SizingType type) {
	WhenResize data = {this, rect, type};
	sendMessageUp(WINDOW_RESIZE, &data);
	WindowEvents::onResize(rect, type);
}

//-----------------------------------------------------------------------------
bool CtrlWindow::onMove(Point_i newPos) {
	WhenMove data = {this, newPos};
	sendMessageUp(WINDOW_MOVE, &data);
	WindowEvents::onMove(newPos);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void* WindowCarcass::sendMessageUp(int32u messageNo, void* data) {
	static int32u depth = 0;
	depth++;
	if (depth == 1) {
		if (messageNo == WINDOW_CREATE) {
			// Запомнить размеры и положение окна
			static int32u count = 0; count++;
			WindowEvents* wnd = data;
			Point_i pos = wnd->getPos();
			Point_i size = wnd->getWindowSize();
			m_positions[wnd] = pos;
			m_sizes[wnd] = size;
			if (count == 1) {
				min = pos;
				max = pos + size;
			}
			min.x = TWG_min(pos.x, min.x);
			min.y = TWG_min(pos.y, min.y);
			max.x = TWG_max(pos.x + size.x, max.x);
			max.y = TWG_max(pos.y + size.y, max.y);
		} else if (messageNo == WINDOW_RESIZE) {
			// Ресайзить и перемещать все окна в соответствии
			WhenResize* data1 = data;
			WindowEvents* wnd = data1->pointer;
			Point_d A = m_positions[wnd];
			Point_d B = A + m_sizes[wnd];
			Point_d A1 = Point_i(data1->rect->ax, data1->rect->ay);
			Point_d B1 = Point_i(data1->rect->bx, data1->rect->by);
			Point_d apos(
				(A.x-min.x)/(max.x-min.x),
				(A.y-min.y)/(max.y-min.y));
			Point_d bpos(
				(B.x-min.x)/(max.x-min.x),
				(B.y-min.y)/(max.y-min.y));
			Point_d max1(
				-(B1.x + A1.x*(bpos.x-1)-apos.x*B1.x)/(apos.x-bpos.x), 
				-(B1.y + A1.y*(bpos.y-1)-apos.y*B1.y)/(apos.y-bpos.y));
			Point_d min1(
				(B1.x*apos.x-A1.x*bpos.x)/(apos.x-bpos.x), 
				(B1.y*apos.y-A1.y*bpos.y)/(apos.y-bpos.y));

			for (i : array)
				if (i != wnd) {
					Point_d C = m_positions[i];
					Point_d D = C + m_sizes[i];
					Point_d cpos(
						(C.x-min.x)/(max.x-min.x),
						(C.y-min.y)/(max.y-min.y));
					Point_d dpos(
						(D.x-min.x)/(max.x-min.x),
						(D.y-min.y)/(max.y-min.y));
					Point_d C1(
						cpos.x*(max1.x-min1.x)+min1.x, 
						cpos.y*(max1.y-min1.y)+min1.y);
					Point_d D1(
						dpos.x*(max1.x-min1.x)+min1.x, 
						dpos.y*(max1.y-min1.y)+min1.y);
					i->setPos(C1);
					i->setWindowSize(D1-C1);
					m_positions[i] = C1;
					m_sizes[i] = D1-C1;
				}

			m_positions[wnd] = A1;
			m_sizes[wnd] = B1-A1;
			max = max1;
			min = min1;
		} else if (messageNo == WINDOW_MOVE) {
			// Перемещать все окна в соответствии
			WhenMove* data1 = data;
			WindowEvents* wnd = data1->pointer;
			Point_i diff = data1->newPos - m_positions[wnd];
			for (i : array)
				if (i != wnd) {
					i->setPos(i->getPos() + diff);
					m_positions[i] = m_positions[i] + diff;
				}
			m_positions[wnd] = data1->newPos;
		}
	}
	depth--;
	return nullptr;	
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

int main() {
	WindowCarcass carcass;
	WindowType type(-1,
		Point_i(100, 100),
		Point_i(200, 100), 
		Point_i(-1, -1),
		Point_i(-1, -1),
		L"Window.",
		WINDOW_TOOL);

	const int32u xOffset = 2;
	const int32u yOffset = 2;
	type.pos = Point_i(100, 100);
	CtrlWindow wnd1(type, &carcass);
	type.pos = type.pos + Point_i(type.size.x + xOffset, 0);
	CtrlWindow wnd2(type, &carcass);
	type.pos = type.pos + Point_i(-type.size.x - xOffset, type.size.y + yOffset);
	CtrlWindow wnd3(type, &carcass);
	type.pos = type.pos + Point_i(type.size.x + xOffset, 0);
	CtrlWindow wnd4(type, &carcass);

	carcass.array.push_back(&wnd1);
	carcass.array.push_back(&wnd2);
	carcass.array.push_back(&wnd3);
	carcass.array.push_back(&wnd4);

	wnd1.waitForClose();
	wnd2.waitForClose();
	wnd3.waitForClose();
	wnd4.waitForClose();
}