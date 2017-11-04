#include <twg/ctrl/brain.h>

namespace twg
{

//-----------------------------------------------------------------------------
BrainCtrl::BrainCtrl(EventsBase* parent) : 
	CtrlBase(parent), 
	m_initialized(false) {
	WindowCtrl** wnd = (WindowCtrl**)sendMessageUp(WINDOW_GET_POINTER, nullptr);
	m_wnd = *wnd;
	delete wnd;	
}

//---------------------------------------------------------------------
bool BrainCtrl::onMessage(int32u messageNo, void* data) {
	if (!m_initialized) {
		m_initialized = true;
		init();
	}
	return onMessageNext(messageNo, data);
}

}