#include "twg/window/window_ctrl.h"

namespace twg
{

//-----------------------------------------------------------------------------
bool WindowCtrl::onMessage(int32u messageNo, void* data) {
	return false;
}

//-----------------------------------------------------------------------------
void* WindowCtrl::sendMessageUp(int32u messageNo, void* data) {

}

//-----------------------------------------------------------------------------
bool WindowCtrl::onMouse(Point_i pos, MouseType type) {

}

//-----------------------------------------------------------------------------
bool WindowCtrl::onKeyboard(KeyType key, bool isDown) {

}

//-----------------------------------------------------------------------------
bool WindowCtrl::onResize(Point_i newSize, Point_i pos, SizingType type) {

}

//-----------------------------------------------------------------------------
bool WindowCtrl::onMove(Point_i newPos) {

}
//-----------------------------------------------------------------------------
bool WindowCtrl::onKillFocus(void) {

}

}