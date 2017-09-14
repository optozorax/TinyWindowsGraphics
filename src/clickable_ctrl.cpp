#include <twg/ctrl/clickable_ctrl.h>

namespace twg
{

//-----------------------------------------------------------------------------
ClickableCtrl::ClickableCtrl(EventsBase* parent) : 
	CtrlBase(parent), 
	m_current(STATE_DEFAULT) {
	WindowCtrl** wnd = (WindowCtrl**)sendMessageUp(WINDOW_GET_POINTER, nullptr);
	m_wnd = *wnd;
	delete wnd;	
}

//-----------------------------------------------------------------------------
bool ClickableCtrl::onMouse(Point_i pos, MouseType type) 
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

	switch (type) {
		case MOUSE_MOVE:
			if (on && m_current != STATE_HOVER && m_current != STATE_CLICK) {
				m_current = STATE_HOVER;
				m_wnd->worthRedraw();
			}
			if (!on && m_current != STATE_DEFAULT) {
				m_current = STATE_DEFAULT;
				m_wnd->worthRedraw();
			}
			break;
		case MOUSE_L_DBL:
		case MOUSE_L_DOWN:
			if (on) {
				m_current = STATE_CLICK;
				isHandle |= true;
			}
			break;
		case MOUSE_L_UP:
			if (m_current == STATE_CLICK) {
				if (on)
					m_current = STATE_HOVER;
				else
					m_current = STATE_DEFAULT;
				onClick();
				isHandle |= true;
			}
			break;
	}
	
	switch (m_current) {
		case STATE_HOVER:
		case STATE_CLICK:
			setCursor(CURSOR_CLICK);
			break;
	}

	return isHandle;
};

//-----------------------------------------------------------------------------
void ClickableCtrl::draw(ImageBase* buffer) {
	switch (m_current) {
		case STATE_DEFAULT:
			drawDefault(buffer);
			break;
		case STATE_CLICK:
			drawWhenClick(buffer);
			break;
		case STATE_HOVER:
			drawHover(buffer);
			break;
	}
}

//-----------------------------------------------------------------------------
bool ClickableCtrl::onFocus(bool isKilled) {
	if (isKilled) { 
		m_current = STATE_DEFAULT;
		m_wnd->worthRedraw();
	}
	return false;
}

}