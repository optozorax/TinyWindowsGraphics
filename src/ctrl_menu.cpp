#include <windows.h>
#include <string>
#include "twg/window/window_ctrl.h"
#include "twg/ctrl/menu.h"


namespace twg
{

//-----------------------------------------------------------------------------
Menu::Menu(std::string str, bool isPopup, Point_i pos, EventsBase* parent) : 
	CtrlBase(parent) {
	WindowCtrl* wnd = sendMessageUp(WINDOW_GET_POINTER, nullptr);
	m_hwnd = wnd->getHwnd();
	makeMenu(str, isPopup);
	m_hmenu = 0;
}

//-----------------------------------------------------------------------------
void Menu::makeMenu(std::string str, bool isPopup) {
	// Удаление старого HMEUN
	// Парсинг str в HMENU
	if (!isPopup)
		SetMenu(m_hwnd, m_hmenu);
}

//-----------------------------------------------------------------------------
Menu::~Menu() {
	DestroyMenu(m_hmenu);
}

//-----------------------------------------------------------------------------
bool Menu::onMessage(int32u messageNo, void* data) {
	const int32u WM_COMAND_MY = int32u(-WM_COMMAND);
	switch (messageNo) {
		case WM_COMAND_MY:
			// Получить номер нажатого пункта
			onClick(0 /* This number. */);
			return true;
	}
}

}