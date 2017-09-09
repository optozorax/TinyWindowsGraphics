#include "twg/window/window_ctrl.h"

namespace twg
{

//-----------------------------------------------------------------------------
void WindowCtrl::worthRedraw(void) {
	m_worthRedraw = true;
}

//-----------------------------------------------------------------------------
bool WindowCtrl::redraw(bool returnType) {
	if (returnType || m_worthRedraw) {
		m_buffer->clear(White);
		storage.draw(m_buffer);
		m_buffer->copyTo(&canvas, Point_i(), Point_i(), m_buffer->size());	
	}
	m_worthRedraw = false;
	return returnType;
}

//-----------------------------------------------------------------------------
bool WindowCtrl::onMessage(int32u messageNo, void* data) {
	if (messageNo == WINDOWS_MESSAGE) {
		onMessageStruct* msg = (onMessageStruct*)data;
		switch (msg->msg) {
			case WM_PAINT: {
				PAINTSTRUCT ps;
				ImageWin img(BeginPaint(msg->hwnd, &ps));

				m_buffer->copyTo(&img, Point_i(), Point_i(), m_buffer->size());

				EndPaint(msg->hwnd, &ps);
				msg->lResult = 0;
				return true;
				} break;

			case WM_ERASEBKGND: {
				msg->lResult = TRUE;
				return true;
				} break;
		}
	}
	return redraw(storage.onMessage(messageNo, data));
}

//-----------------------------------------------------------------------------
void* WindowCtrl::sendMessageUp(int32u messageNo, void* data) {
	if (messageNo == WINDOW_GET_POINTER)
		return new WindowCtrl*(this);
	else if (messageNo == CTRL_GET_POINTER) // Хранилище пошлет такое сообщение, а так как окно не является хранилищем, то и возвращать надо такое
		return nullptr;
	else
		return EventsBase::sendMessageUp(messageNo, data);
	
}

//-----------------------------------------------------------------------------
bool WindowCtrl::onMouse(Point_i pos, MouseType type) {
	setCursor(CURSOR_STANDART);
	return redraw(storage.onMouse(pos, type));
}

//-----------------------------------------------------------------------------
bool WindowCtrl::onKeyboard(KeyType key, bool isDown) {
	return redraw(storage.onKeyboard(key, isDown));
}

//-----------------------------------------------------------------------------
bool WindowCtrl::onResize(Rect rect, SizingType type) {
	bool isResized = false;
	Point_i newSize(rect.x(), rect.y()); 
	Point_i pos(rect.ax, rect.ay);
	if (newSize.x > m_buffer->width() || newSize.y > m_buffer->height()) {
		if (newSize.x > m_buffer->width())
			m_buffer->resize(Point_i(newSize.x + 100, m_buffer->height()));
		if (newSize.y > m_buffer->height())
			m_buffer->resize(Point_i(m_buffer->width(), newSize.y + 100));
		isResized = true;
	}
	return redraw(storage.onResize(rect, type) || isResized);
}

//-----------------------------------------------------------------------------
bool WindowCtrl::onMove(Point_i newPos) {
	return redraw(storage.onMove(newPos));

}
//-----------------------------------------------------------------------------
bool WindowCtrl::onFocus(bool isKilled) {
	return redraw(storage.onFocus(isKilled));
}

}