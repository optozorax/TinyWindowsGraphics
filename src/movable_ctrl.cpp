#include <twg/ctrl/movable_ctrl.h>

namespace twg
{

//-----------------------------------------------------------------------------
MovableCtrl::MovableCtrl(EventsBase* parent) : 
	CtrlBase(parent), 
	m_current(STATE_DEFAULT) {
	WindowCtrl** wnd = (WindowCtrl**)sendMessageUp(WINDOW_GET_POINTER, nullptr);
	m_wnd = *wnd;
	delete wnd;	
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
				m_wnd->worthRedraw();
			}
			if (!on && (m_current != STATE_MOVE) && (m_current != STATE_DEFAULT)) {
				m_current = STATE_DEFAULT;
				m_wnd->worthRedraw();
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

//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
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

}