#include <windows.h>
#include <cstdlib>
#include <sstream>
#include "twg/twg.h"
#include "twg/window/window_ctrl.h"
#include "twg/image/image_drawing.h"
#include "twg/ctrl/menu.h"

using namespace twg;

//-----------------------------------------------------------------------------
class MovableCtrl : public CtrlBase 
{
public:
	MovableCtrl(EventsBase* parent) : 
		CtrlBase(parent), 
		m_current(STATE_DEFAULT) {}

	//-------------------------------------------------------------------------
	bool onMouse(Point_i pos, MouseType type) final;
	bool onKeyboard(KeyType key, bool isDown) final;
	void draw(ImageBase* buffer) final;
protected:
	virtual bool isInside(Point_i pos) = 0;
	virtual void move(Point_i diff) = 0;

	virtual bool onRMouse(Point_i pos) {};
	virtual bool onWheel(Point_i pos, MouseType wheel) {};

	virtual void drawDefault(ImageBase* buffer) {};
	virtual void drawHover(ImageBase* buffer) { drawDefault(buffer); };
	virtual void drawWhenMove(ImageBase* buffer) { drawDefault(buffer); };

private:
	enum MyState
	{
		STATE_DEFAULT,
		STATE_MOVE,
		STATE_HOVER
	};

	MyState			m_current;
	Point_i 		m_lastPos;
};

//-----------------------------------------------------------------------------
class PointCtrl : public MovableCtrl
{
public:
	PointCtrl(Point_d pos, EventsBase* parent) : 
		MovableCtrl(parent), 
		m_pos(pos) {
			if (!m_drawed)
				redrawBuffers();
			m_storageParent = sendMessageUp(CTRL_GET_POINTER, nullptr);
		}

	~PointCtrl() {
		delete m_storageParent;
	}

	bool onMessage(int32u messageNo, void* data);
private:
	Point_d 				m_pos;
	CtrlStorage**			m_storageParent;

	static int32u 			m_radius;
	static int32u 			m_minRadius;
	static int32u 			m_maxRadius;

	static bool				m_drawed;
	static ImageDrawing_aa  m_imgMove;
	static ImageDrawing_aa  m_imgHover;
	static ImageDrawing_aa  m_imgDefault;

	void redrawBuffers();

	//-------------------------------------------------------------------------
	bool isInside(Point_i pos);
	void move(Point_i diff);

	bool onRMouse(Point_i pos);
	bool onWheel(Point_i pos, MouseType wheel);

	void drawDefault(ImageBase* buffer);
	void drawHover(ImageBase* buffer);
	void drawWhenMove(ImageBase* buffer);
};

//=============================================================================
//=============================================================================
//=============================================================================

//-----------------------------------------------------------------------------

extern int32u 			PointCtrl::m_radius(5);
extern int32u			PointCtrl::m_minRadius(3);
extern int32u 			PointCtrl::m_maxRadius(30);

extern ImageDrawing_aa  PointCtrl::m_imgDefault(Point_i(10, 10));
extern ImageDrawing_aa  PointCtrl::m_imgHover(Point_i(10, 10));
extern ImageDrawing_aa  PointCtrl::m_imgMove(Point_i(10, 10));
extern bool 			PointCtrl::m_drawed(false);

void PointCtrl::redrawBuffers() {
	Point_d r(m_radius, m_radius);
	m_imgMove.resize(Point_i(r*2 + Point_d(6, 6)));
	m_imgHover.resize(Point_i(r*2 + Point_d(6, 6)));
	m_imgDefault.resize(Point_i(r*2 + Point_d(6, 6)));

	Polygon_d poly = computeEllipse(r);
	poly.move(r + Point_d(3, 3));

	m_imgDefault.clear(Transparent);
	m_imgHover.clear(Transparent);
	m_imgMove.clear(Transparent);

	m_imgDefault.setBrush(Brush(setAlpha(White, 128)));
	m_imgDefault.setPen(Pen(1, Red));
	m_imgDefault.drawPolygon(poly);
	m_imgDefault.drawPolyline(poly);

	m_imgHover.setBrush(Brush(setAlpha(Orange, 128)));
	m_imgHover.drawPolygon(poly);

	m_imgMove.setBrush(Brush(setAlpha(Orange, 50)));
	m_imgMove.drawPolygon(poly);

	m_drawed = true;
}

void PointCtrl::move(Point_i diff) {
	m_pos += diff;
}

bool PointCtrl::isInside(Point_i pos) {
	return (m_pos-pos).getLength() < m_radius;
}

bool PointCtrl::onRMouse(Point_i pos) {
	std::wstringstream sout;
	sout << L"=" << (*m_storageParent)->getId(this) << L" Удалить точку | --- | ~ Координаты: (" << m_pos.x << L", " << m_pos.y << L") | ~ Радиус: " << m_radius;
	
	Menu* menu = new Menu(sout.str(), true, m_pos, m_parent);
	(*m_storageParent)->array.push_back(menu);
	return true;
}

bool PointCtrl::onMessage(int32u messageNo, void* data) {
	bool isMenu = messageNo == MENU_CLICK;
	bool isThisId = (*(int32u*)(data)) == (*m_storageParent)->getId(this);
	if (isMenu && isThisId) {
		(*m_storageParent)->deleteMe(this);
	}
	return false;
}

bool PointCtrl::onWheel(Point_i pos, MouseType wheel) {
	switch (wheel) {
		case MOUSE_WHEEL_UP:
			if (m_radius < m_maxRadius) {
				m_radius++;
				redrawBuffers();
			}
			break;
		case MOUSE_WHEEL_DOWN:
			if (m_radius > m_minRadius) {
				m_radius--;
				redrawBuffers();
			}
			break;
	}
	return true;
}

void PointCtrl::drawDefault(ImageBase* buffer) {
	m_imgDefault.drawTo(buffer, m_pos - Point_i(m_radius+3, m_radius+3), Point_i(0, 0), m_imgDefault.size());	
}

void PointCtrl::drawHover(ImageBase* buffer) {
	m_imgHover.drawTo(buffer, m_pos - Point_i(m_radius+3, m_radius+3), Point_i(0, 0), m_imgHover.size());	
}

void PointCtrl::drawWhenMove(ImageBase* buffer) {
	m_imgMove.drawTo(buffer, m_pos - Point_i(m_radius+3, m_radius+3), Point_i(0, 0), m_imgMove.size());	
}

//-----------------------------------------------------------------------------

bool MovableCtrl::onMouse(Point_i pos, MouseType type) 
{
	bool isHandle = false;
	bool on = isInside(pos);

	if (m_current == STATE_HOVER) {
		switch (type) {
			case MOUSE_R_UP:
				isHandle |= onRMouse(pos);
				break;
			case MOUSE_WHEEL_UP:
			case MOUSE_WHEEL_DOWN:
				isHandle |= onWheel(pos, type);
				break;
		}
	}

	if (m_current == STATE_MOVE && type == MOUSE_MOVE) {
		move(pos-m_lastPos);
		m_lastPos = pos;
		isHandle |= true;
	}

	switch (type) {
		case MOUSE_MOVE:
			if (on && !(m_current == STATE_MOVE)) {
				m_current = STATE_HOVER;
				isHandle |= true;
			}
			if (!on && (m_current != STATE_MOVE) && (m_current != STATE_DEFAULT)) {
				m_current = STATE_DEFAULT;
				isHandle |= true;
			}
			break;
		case MOUSE_L_DOWN:
			if (on) {
				m_lastPos = pos;
				m_current = STATE_MOVE;
				isHandle |= true;
			}
			break;
		case MOUSE_L_UP:
			if (m_current == STATE_MOVE) {
				m_current = STATE_HOVER;
				isHandle |= true;
			}
			break;
	}
	
	switch (m_current) {
		case STATE_HOVER:
			setCursor(CURSOR_CLICK);
			break;
		case STATE_MOVE:
			setCursor(CURSOR_CLICK);
			break;
	}

	return isHandle;
};

bool MovableCtrl::onKeyboard(KeyType key, bool isDown) 
{
	if (isDown && (m_current == STATE_MOVE || m_current == STATE_HOVER))
	switch (key) {
		case VK_UP:
			move(Point_i(0, -1));
			m_lastPos += Point_i(0, -1);
			return true;
		case VK_DOWN:
			move(Point_i(0, 1));
			m_lastPos += Point_i(0, 1);
			return true;
		case VK_LEFT:
			move(Point_i(-1, 0));
			m_lastPos += Point_i(-1, 0);
			return true;
		case VK_RIGHT:
			move(Point_i(1, 0));
			m_lastPos += Point_i(1, 0);
			return true;
	}

	return false;
}

void MovableCtrl::draw(ImageBase* buffer) {
	switch (m_current) {
		case STATE_DEFAULT:
			drawDefault(buffer);
			break;
		case STATE_MOVE:
			drawWhenMove(buffer);
			break;
		case STATE_HOVER:
			drawHover(buffer);
			break;
	}
}

//-----------------------------------------------------------------------------

int main() {
	WindowType type(-1,
		Point_i(100, 0),
		Point_i(300, 300), 
		Point_i(-1, -1),
		Point_i(-1, -1),
		L"There are 500 points.",
		WINDOW_STANDART);	
	WindowCtrl wnd(type);

	wnd.storage.OMFOC = true;
	for (int i = 0; i < 500; ++i) {
		Point_i size = wnd.getClientSize();
		Point_i pos(std::rand()*size.x/RAND_MAX, std::rand()*size.y/RAND_MAX);
		wnd.storage.array.push_back(new PointCtrl(pos, &wnd.storage));
	}

	wnd.storage.array.push_back(new Menu(L"Файл > =1 #1 Новый\tCtrl+N | =2 #2 Открыть\tCtrl+O | =3 #3 Сохранить\tCtrl+S | --- | Последние > =4 ! 1.c | =5 2.c | =6 $ 3.c | =7 \t4.c | =8 ~ 5.c < --- | =9 #5 Выход\tAlt+F4 < Правка > =10 ... <", false, Point_i(0, 0), &wnd.storage));

	wnd.redraw(true);

	wnd.waitForClose();
	return 0;
}