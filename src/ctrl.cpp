#include "twg/ctrl.h"

namespace twg
{

//-----------------------------------------------------------------------------
CtrlBase::CtrlBase(EventsBase* parent) : EventsHandler(parent), m_storage(nullptr), id(0) {
	CtrlStorage** storage = (CtrlStorage**)sendMessageUp(CTRL_GET_POINTER, nullptr);
	if (storage != nullptr) {
		m_storage = *storage;
		delete storage;
		id = m_storage->getId(this);
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
int32u CtrlStorage::IdDistributor::getId(void* pointer) {
	// Находим, есть ли указатель в массиве
	for (auto& i : m_ids)
		if (i.first == pointer)
			return i.second;

	// Если его нет, выделяем ему айди из раннее освободившихся
	if (!m_freeId.empty()) {
		int32u id = m_freeId.back();
		m_ids.push_back(std::pair<void*, int32u>(pointer, id));
		m_freeId.pop_back();
		return id;
	}

	// И только когда ни то, ни другое, создаем новое айди
	int32u id = m_ids.size();
	m_ids.push_back(std::pair<void*, int32u>(pointer, id));
	return id;
}

//-----------------------------------------------------------------------------
void CtrlStorage::IdDistributor::deleteId(void* pointer) {
	int32u pos = int32u(-1);
	for (auto i = m_ids.begin(); i != m_ids.end(); ++i)
		if (i->first == pointer)
			pos = i - m_ids.begin();
	if (pos == int32u(-1)) return;
	int32u id = m_ids[pos].second;
	m_freeId.push_back(id);
	m_ids.erase(m_ids.begin() + pos);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
int32u CtrlStorage::getId(CtrlBase* ctrl) {
	return m_distributor.getId(ctrl);
}

//-----------------------------------------------------------------------------
int32u CtrlStorage::getPos(CtrlBase* ctrl) {
	for (int i = 0; i < array.size(); ++i)
		if (array[i] == ctrl)
			return i;
	return int32u(-1);
}

//-----------------------------------------------------------------------------
void CtrlStorage::deleteMe(CtrlBase* ctrl) {
	int32u pos = getPos(ctrl);
	if (pos != int32u(-1)) {
		pos = int32u(-1);
		for (int i = 0; i < m_toDelete.size(); ++i)
			if (m_toDelete[i] == ctrl)
				pos = i;
		if (pos == int32u(-1))
			m_toDelete.push_back(ctrl);
	}
}

//-----------------------------------------------------------------------------
void CtrlStorage::deleteCtrls(void) {
	while (m_toDelete.size() != 0) {
		auto ctrl = m_toDelete.back();
		m_distributor.deleteId(ctrl);
		int32u id = getPos(ctrl);
		auto position = array.begin() + id;
		array.erase(position);
		delete m_toDelete.back();
		m_toDelete.pop_back();
	}
}

//-----------------------------------------------------------------------------
void CtrlStorage::draw(ImageBase* buffer) {
	for (auto i = array.rbegin(); i != array.rend(); ++i)
		(*i)->draw(buffer);
}

//-----------------------------------------------------------------------------
template<class Function>
bool CtrlStorage::sendMessage(Function f) {
	/* Это функция, которая посылает всем контролам какое-то сообщение, заданное функцией f. Она написана таким образом, чтобы учесть случаи, когда массив контролов модифицируется во время вызова сообщения. m_messageDepth нужно для того, чтобы контролы удалялись только к полному выходу из отправки сообщений. Иначе может случиться так, что на глубине, например, 2, мы удалили почти весь массив контролов, и когда будет на глубине 1, то мы будем обращаться к указателям на эти удаленные контролы и произойдет ошибка сегментации. */
	m_messageDepth++;
	std::vector<CtrlBase*> arrayCopy(array.begin(), array.end());
	bool isOneTrue = false;
	for (auto& i : arrayCopy) {
		bool returned = f(i);
		isOneTrue |= returned;
		if (returned && OMFOC) {
			m_messageDepth--;
			if (m_messageDepth == 0) deleteCtrls();
			return true;
		}
	}
	m_messageDepth--;
	if (m_messageDepth == 0) deleteCtrls();
	return isOneTrue;
}

//-----------------------------------------------------------------------------
bool CtrlStorage::onMouse(Point_i pos, MouseType type) {
	return sendMessage([&] (CtrlBase* i) -> bool { 
		return i->onMouse(pos, type); 
	});
}

//-----------------------------------------------------------------------------
bool CtrlStorage::onKeyboard(KeyType key, bool isDown) {
	return sendMessage([&] (CtrlBase* i) -> bool { 
		return i->onKeyboard(key, isDown); 
	});
}

//-----------------------------------------------------------------------------
bool CtrlStorage::onResize(Rect rect, SizingType type) {
	return sendMessage([&] (CtrlBase* i) -> bool { 
		return i->onResize(rect, type); 
	});
}

//-----------------------------------------------------------------------------
bool CtrlStorage::onMove(Point_i newPos) {
	return sendMessage([&] (CtrlBase* i) -> bool { 
		return i->onMove(newPos); 
	});
}

//-----------------------------------------------------------------------------
bool CtrlStorage::onKillFocus(void) {
	return sendMessage([&] (CtrlBase* i) -> bool { 
		return i->onKillFocus(); 
	});
}

//-----------------------------------------------------------------------------
bool CtrlStorage::onMessage(int32u messageNo, void* data) {
	return sendMessage([&] (CtrlBase* i) -> bool { 
		return i->onMessage(messageNo, data); 
	});
}

//-----------------------------------------------------------------------------
void* CtrlStorage::sendMessageUp(int32u messageNo, void* data) {
	if (messageNo == CTRL_GET_POINTER) {
		return new CtrlStorage*(this);
	} else
		return EventsBase::sendMessageUp(messageNo, data);
}

}