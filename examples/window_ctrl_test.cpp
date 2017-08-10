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
	static ImageDrawing_win m_imgMove;
	static ImageDrawing_win m_imgHover;
	static ImageDrawing_win m_imgDefault;

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
extern int32u			PointCtrl::m_minRadius(2);
extern int32u 			PointCtrl::m_maxRadius(300);

extern ImageDrawing_win PointCtrl::m_imgDefault(Point_i(1, 1));
extern ImageDrawing_win PointCtrl::m_imgHover(Point_i(1, 1));
extern ImageDrawing_win PointCtrl::m_imgMove(Point_i(1, 1));
extern bool 			PointCtrl::m_drawed(false);

void PointCtrl::redrawBuffers() {
	m_imgMove.resize(Point_i(m_radius, m_radius)*2 + Point_i(1, 1));
	m_imgHover.resize(Point_i(m_radius, m_radius)*2 + Point_i(1, 1));
	m_imgDefault.resize(Point_i(m_radius, m_radius)*2 + Point_i(1, 1));

	m_imgDefault.clear(Transparent);
	m_imgHover.clear(Transparent);
	m_imgMove.clear(Transparent);

	m_imgDefault.setBrush(White);
	m_imgDefault.setPen(Pen(1, Red));
	Ellipse(m_imgDefault.getHdc(), 0, 0, m_radius * 2, m_radius * 2);

	m_imgHover.setBrush(Orange);
	m_imgHover.setPen(Pen(1, Transparent));
	Ellipse(m_imgHover.getHdc(), 0, 0, m_radius * 2, m_radius * 2);

	m_imgMove.setBrush(getColorBetween(0.5, Orange, White));
	m_imgMove.setPen(Pen(1, Transparent));
	Ellipse(m_imgMove.getHdc(), 0, 0, m_radius * 2, m_radius * 2);

	PointIterator<Point_i> i(Point_i(0, 0), m_imgMove.size() - Point_i(1, 1));
	for (i.onStart(); !i.isEnd(); ++i) {
		if (m_imgMove[*i] != 0)
			m_imgMove[*i] = setAlpha(m_imgMove[*i], 255);
		if (m_imgHover[*i] != 0)
			m_imgHover[*i] = setAlpha(m_imgHover[*i], 255);
		if (m_imgDefault[*i] != 0)
			m_imgDefault[*i] = setAlpha(m_imgDefault[*i], 255);
	}

	m_drawed = true;
}

void PointCtrl::move(Point_i diff) {
	m_pos += diff;
}

bool PointCtrl::isInside(Point_i pos) {
	return (m_pos-pos).getLength() < m_radius;
}

bool PointCtrl::onRMouse(Point_i pos) {
	// CtrlStorage** parent = sendMessageUp(CTRL_GET_POINTER, nullptr);
	// (*parent)->deleteMe(this);
	// delete parent;
	// return true;
	

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
	m_imgDefault.drawTo(buffer, m_pos - Point_i(m_radius, m_radius), Point_i(0, 0), m_imgDefault.size());	
}

void PointCtrl::drawHover(ImageBase* buffer) {
	m_imgHover.drawTo(buffer, m_pos - Point_i(m_radius, m_radius), Point_i(0, 0), m_imgHover.size());	
}

void PointCtrl::drawWhenMove(ImageBase* buffer) {
	m_imgMove.drawTo(buffer, m_pos - Point_i(m_radius, m_radius), Point_i(0, 0), m_imgMove.size());	
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

	// 

	wnd.redraw(true);

	wnd.waitForClose();
	return 0;
}