#include <windows.h>
#include <string>
#include "twg/window/window_ctrl.h"
#include "twg/ctrl/menu.h"

namespace twg
{

//-----------------------------------------------------------------------------
namespace MenuParser 
{

void parseMenu(std::wstring str, HMENU menu);

void deleteSpaces(std::wstring& str);
void parseMenuItem(std::wstring str, HMENU menu, bool isPopup = false, HMENU popupMenu = 0);
void parseMenuPopupItem(std::wstring str, std::wstring str2, HMENU menu);
void parseMenuNoRight(std::wstring str, HMENU menu);
void parseMenuNoUp(std::wstring str, HMENU menu);
HBITMAP icon2bmp(HICON hicon);

}

//=============================================================================
//=============================================================================
//=============================================================================

//-----------------------------------------------------------------------------
void MenuParser::deleteSpaces(std::wstring& str) {
	while (str[str.size()-1] == L' ') str.erase(str.size()-1, 1);
	while (str[0] == L' ') str.erase(0, 1);
}

//-----------------------------------------------------------------------------
void MenuParser::parseMenuItem(std::wstring str, HMENU menu, bool isPopup, HMENU popupMenu) {
	deleteSpaces(str);

	// Узнать, сепаратор ли это
	bool separator = str == L"---";

	// Узнать номер пункта меню
	deleteSpaces(str);
	size_t posEqual = str.find(L"=");
	int32u id = 0;
	if (posEqual != std::wstring::npos) {
		size_t posSpace = str.find(L" ");
		std::wstring num = str.substr(posEqual+1, posSpace-posEqual-1);
		id = stoull(num);
		str.erase(0, posSpace+1);
	}

	// Узнать номер картинки
	deleteSpaces(str);
	size_t posSharp = str.find(L"#");
	int32u pictureNo = -1;
	if (posSharp != std::wstring::npos) {
		size_t posSpace = str.find(L" ");
		std::wstring num = str.substr(posSharp+1, posSpace-posSharp-1);
		pictureNo = stoull(num);
		str.erase(0, posSpace+1);
	}

	// Выявить Checked
	deleteSpaces(str);
	bool checked = false;
	if (str[0] == L'!') {
		checked = true;
		str.erase(0, 1);
	}

	// Выявить MenuBarBreak
	deleteSpaces(str);
	bool menuBarBreak = false;
	if (str[0] == L'$') {
		menuBarBreak = true;
		str.erase(0, 1);
	}

	// Выявить Grayed
	deleteSpaces(str);
	bool grayed = false;
	if (str[0] == L'~') {
		grayed = true;
		str.erase(0, 1);
	}

	wchar_t name[100] = {};
	wcscpy(name, str.c_str());
	MENUITEMINFO minfo = {};
	minfo.cbSize = sizeof(MENUITEMINFO);
	minfo.fMask = MIIM_FTYPE | MIIM_ID | MIIM_STATE | MIIM_STRING; 
	if (pictureNo != -1)
		minfo.fMask |= MIIM_BITMAP;
	if (isPopup)
		minfo.fMask |= MIIM_SUBMENU;
	if (separator)
		minfo.fType = MFT_SEPARATOR;
	if (menuBarBreak)
		minfo.fType = MFT_MENUBARBREAK;
	if (checked)
		minfo.fState = MFS_CHECKED;
	if (grayed)
		minfo.fState |= MFS_GRAYED;
	minfo.wID = id;
	if (isPopup)
		minfo.hSubMenu = popupMenu;
	minfo.dwTypeData = name;
	minfo.cch = str.size();
	if (pictureNo != -1)
		minfo.hbmpItem = icon2bmp(LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(pictureNo)));

	InsertMenuItem(menu, GetMenuItemCount(menu), TRUE, &minfo);
}

//-----------------------------------------------------------------------------
void MenuParser::parseMenuPopupItem(std::wstring str, std::wstring str2, HMENU menu) {
	deleteSpaces(str);
	HMENU popupMenu = CreatePopupMenu();
	parseMenu(str2, popupMenu);
	parseMenuItem(str, menu, true, popupMenu);
}

//-----------------------------------------------------------------------------
void MenuParser::parseMenuNoRight(std::wstring str, HMENU menu) {
	size_t posUp = str.find(L"|");
	std::wstring strItem = str.substr(0, posUp);
	parseMenuItem(strItem, menu);

	str.erase(0, posUp+1);
	parseMenu(str, menu);
}

//-----------------------------------------------------------------------------
void MenuParser::parseMenuNoUp(std::wstring str, HMENU menu) {
	size_t posRight = str.find(L">");
	int32 count = 0;
	int32u posLeft = 0;
	for (int32u i = posRight; i < str.size(); ++i) {
		if (str[i] == L'>') count++;
		if (str[i] == L'<') count--;
		if (count == 0) {
			posLeft = i;
			break;
		}
	}
	std::wstring strItem = str.substr(0, posRight);
	std::wstring strPopupItem = str.substr(posRight+1, posLeft-posRight-1);
	parseMenuPopupItem(strItem, strPopupItem, menu);

	str.erase(0, posLeft+1);
	parseMenu(str, menu);
}

//-----------------------------------------------------------------------------
void MenuParser::parseMenu(std::wstring str, HMENU menu) {
	size_t posRight = str.find(L">");
	size_t posUp = str.find(L"|");

	bool noRight = posRight == std::wstring::npos;
	bool noUp = posUp == std::wstring::npos;
	if (noRight && noUp) {
		parseMenuItem(str, menu);
	} else if (noRight && !noUp) {
		parseMenuNoRight(str, menu);
	} else if (!noRight && noUp) {
		parseMenuNoUp(str, menu);
	}  else {
		if (posUp < posRight)
			parseMenuNoRight(str, menu);
		else
			parseMenuNoUp(str, menu);
	}
}

//-----------------------------------------------------------------------------
HBITMAP MenuParser::icon2bmp(HICON hicon) {
	/* Преобразует иконку hicon в формат точечного рисунка, причем рисунок имеет размеры 18x18 пикселей, и его фон такой же, как и у пункта меню. */
	HBITMAP hbmp;
	HDC hdcBmp;
	HBRUSH menuBrush;
	MENUITEMINFO minf;        

	hdcBmp = CreateCompatibleDC(GetDC(NULL));

	menuBrush = CreateSolidBrush(GetSysColor(COLOR_MENU));
	SelectObject(hdcBmp, menuBrush); 
	
	hbmp = CreateCompatibleBitmap(GetDC(NULL), 18, 18); 
	SelectObject(hdcBmp, hbmp);

	ExtFloodFill(hdcBmp, 0, 0, GetSysColor(COLOR_MENU), FLOODFILLBORDER); 

	SelectObject(hdcBmp, hbmp); 
	DrawIconEx(hdcBmp, 1,1, hicon, 16,16, NULL, CreateSolidBrush( GetSysColor(COLOR_MENU) ), DI_NORMAL | DI_COMPAT);

	DeleteDC(hdcBmp);

	return hbmp;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
StaticMenu::StaticMenu(std::wstring str, EventsBase* parent) : CtrlBase(parent), m_hmenu(0) {
	WindowCtrl** wnd = (WindowCtrl**)sendMessageUp(WINDOW_GET_POINTER, nullptr);
	m_wnd = *wnd;
	delete wnd;	

	change(str);
}

//-----------------------------------------------------------------------------
StaticMenu::~StaticMenu() {
	DestroyMenu(m_hmenu);
};

//-----------------------------------------------------------------------------
void StaticMenu::change(std::wstring str) {
	if (m_hmenu != 0)
		DestroyMenu(m_hmenu);

	m_hmenu = CreateMenu();
	MenuParser::parseMenu(str, m_hmenu);
	SetMenu(m_wnd->getHwnd(), m_hmenu);
}

//-----------------------------------------------------------------------------
void StaticMenu::onClick(int32u id) {
	sendMessageUp(MENU_CLICK, &id);
}

//-----------------------------------------------------------------------------
bool StaticMenu::onMessage(int32u messageNo, void* data) {
	if (messageNo == WINDOWS_MESSAGE) {
		onMessageStruct* msg = (onMessageStruct*)data;
		if (msg->msg == WM_COMMAND) {
			onClick(LOWORD(msg->wParam));
			m_wnd->redraw();
		}

	}

	// Всегда возвращается false - для того, чтобы другие Popup меню могли получить сообщение. Это костыль
	return false;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
PopupMenu::PopupMenu(EventsBase* parent) : CtrlBase(parent), m_hmenu(0) {
	WindowCtrl** wnd = (WindowCtrl**)sendMessageUp(WINDOW_GET_POINTER, nullptr);
	m_wnd = *wnd;
	delete wnd;	
}

//-----------------------------------------------------------------------------
int32u PopupMenu::show(std::wstring str, Point_i pos) {
	m_hmenu = CreatePopupMenu();
	MenuParser::parseMenu(str, m_hmenu);
	pos = m_wnd->client2global(pos);
	return TrackPopupMenuEx(m_hmenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD | TPM_NONOTIFY, pos.x, pos.y, m_wnd->getHwnd(), NULL);
}

//-----------------------------------------------------------------------------
bool PopupMenu::onMessage(int32u messageNo, void* data) {
	if (messageNo == WINDOWS_MESSAGE) {
		onMessageStruct* msg = (onMessageStruct*)data;
		switch (msg->msg) {
			// Обрабатываем эти сообщения, чтобы во время разрушения меню, оно могло само себя удалить
			case WM_UNINITMENUPOPUP:
				if (HMENU(msg->wParam) == m_hmenu) {
					m_storage->deleteMe(this);
					m_hmenu = 0;
					msg->lResult = 0;
					return true;
				} break;
			case WM_INITMENUPOPUP:
				if (HMENU(msg->wParam) == m_hmenu) {
					msg->lResult = 0;
					return true;
				} break;
		}
	}
	return false;	
}

}