#include "twg/ctrl.h"

namespace twg
{

//-----------------------------------------------------------------------------
int32u CtrlStorage::getId(CtrlBase* ctrl) {
	for (int i = 0; i < array.size(); ++i)
		if (array[i] == ctrl)
			return i;
	return int32u(-1);
}

//-----------------------------------------------------------------------------
void CtrlStorage::deleteMe(CtrlBase* ctrl) {
	int32u id = getId(ctrl);
	if (id != int32u(-1))
		m_toDelete.push_back(ctrl);
}

//-----------------------------------------------------------------------------
void CtrlStorage::deleteCtrls(void) {
	while (m_toDelete.size() != 0) {
		auto ctrl = m_toDelete.back();
		int32u id = getId(ctrl);
		auto position = array.begin() + id;
		array.erase(position);
		delete m_toDelete.back();
		m_toDelete.pop_back();
	}
}

//-----------------------------------------------------------------------------
void CtrlStorage::draw(ImageBase* buffer) {
	deleteCtrls();
	
	for (auto i = array.rbegin(); i != array.rend(); ++i)
		(*i)->draw(buffer);
}

//-----------------------------------------------------------------------------
bool CtrlStorage::onMouse(Point_i pos, MouseType type) {
	for (i : array)
		if (i->onMouse(pos, type) && OMFOC)
			return true;
	return false;
}

//-----------------------------------------------------------------------------
bool CtrlStorage::onKeyboard(KeyType key, bool isDown) {
	for (i : array)
		if (i->onKeyboard(key, isDown) && OMFOC)
			return true;
	return false;
}

//-----------------------------------------------------------------------------
bool CtrlStorage::onResize(Point_i newSize, Point_i pos, SizingType type) {
	for (i : array)
		if (i->onResize(newSize, pos, type) && OMFOC)
			return true;
	return false;
}

//-----------------------------------------------------------------------------
bool CtrlStorage::onMove(Point_i newPos) {
	for (i : array)
		if (i->onMove(newPos) && OMFOC)
			return true;
	return false;
}

//-----------------------------------------------------------------------------
bool CtrlStorage::onKillFocus(void) {
	for (i : array)
		if (i->onKillFocus() && OMFOC)
			return true;
	return false;
}

//-----------------------------------------------------------------------------
bool CtrlStorage::onMessage(int32u messageNo, void* data) {
	for (i : array)
		if (i->onMessage(messageNo, data) && OMFOC)
			return true;
	return false;
}

//-----------------------------------------------------------------------------
void* CtrlStorage::sendMessageUp(int32u messageNo, void* data) {
	if (messageNo == CTRL_GET_POINTER) {
		return new CtrlStorage*(this);
	} else
		return EventsBase::sendMessageUp(messageNo, data);
}

}