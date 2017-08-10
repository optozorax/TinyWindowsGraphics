#include <windows.h>
#include <string>
#include "twg/window/window_ctrl.h"
#include "twg/ctrl/menu.h"

namespace twg
{

//-----------------------------------------------------------------------------
Menu::Menu(std::wstring str, bool isPopup, Point_i pos, EventsBase* parent) : 
	CtrlBase(parent) {

	WindowCtrl** wnd = sendMessageUp(WINDOW_GET_POINTER, nullptr);
	m_hwnd = (*wnd)->getHwnd();
	pos = (*wnd)->client2global(pos);
	delete wnd;

	m_hmenu = 0;
	makeMenu(str, isPopup);


	if (isPopup)
		TrackPopupMenuEx(m_hmenu, TPM_LEFTALIGN | TPM_TOPALIGN, 
		pos.x, pos.y, m_hwnd, NULL);
}

//-----------------------------------------------------------------------------
void Menu::deleteSpaces(std::wstring& str) {
	while (str[str.size()-1] == L' ') str.erase(str.size()-1, 1);
	while (str[0] == L' ') str.erase(0, 1);
}

//-----------------------------------------------------------------------------
void Menu::parseMenuItem(std::wstring str, HMENU menu, bool isPopup = false, HMENU popupMenu = 0) {
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
		id = stoi(num);
		str.erase(0, posSpace+1);
	}

	// Узнать номер картинки
	deleteSpaces(str);
	size_t posSharp = str.find(L"#");
	int32u pictureNo = -1;
	if (posSharp != std::wstring::npos) {
		size_t posSpace = str.find(L" ");
		std::wstring num = str.substr(posSharp+1, posSpace-posSharp-1);
		pictureNo = stoi(num);
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
	minfo.dwTypeData = str.c_str();
	minfo.cch = str.size();
	if (pictureNo != -1)
		minfo.hbmpItem = icon2bmp(LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(pictureNo)));

	InsertMenuItem(menu, GetMenuItemCount(menu), TRUE, &minfo);
}

//-----------------------------------------------------------------------------
void Menu::parseMenuPopupItem(std::wstring str, std::wstring str2, HMENU menu) {
	deleteSpaces(str);
	HMENU popupMenu = CreatePopupMenu();
	parseMenu(str2, popupMenu);
	parseMenuItem(str, menu, true, popupMenu);
}

//-----------------------------------------------------------------------------
void Menu::parseMenuNoRight(std::wstring str, HMENU menu) {
	size_t posUp = str.find(L"|");
	std::wstring strItem = str.substr(0, posUp);
	parseMenuItem(strItem, menu);

	str.erase(0, posUp+1);
	parseMenu(str, menu);
}

//-----------------------------------------------------------------------------
void Menu::parseMenuNoUp(std::wstring str, HMENU menu) {
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
void Menu::parseMenu(std::wstring str, HMENU menu) {
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
void Menu::makeMenu(std::wstring str, bool isPopup) {
	if (m_hmenu != 0)
		DestroyMenu(m_hmenu);
	
	if (isPopup)
		m_hmenu = CreatePopupMenu();
	else
		m_hmenu = CreateMenu();

	parseMenu(str, m_hmenu);

	if (!isPopup)
		SetMenu(m_hwnd, m_hmenu);
}

//-----------------------------------------------------------------------------
Menu::~Menu() {
	DestroyMenu(m_hmenu);
}

//-----------------------------------------------------------------------------
bool Menu::onMessage(int32u messageNo, void* data) {
	if (messageNo == WINDOWS_MESSAGE) {
		onMessageStruct* msg = data;
		switch (msg->msg) {
			case WM_COMMAND: {
				onClick(LOWORD(msg->wParam));
				msg->lResult = 0;
				return true;
				} break;
			case WM_UNINITMENUPOPUP: {
				if (msg->wParam == m_hmenu) {
					CtrlStorage** storage = sendMessageUp(CTRL_GET_POINTER, nullptr);
					(*storage)->deleteMe(this);
					delete storage;
					msg->lResult = 0;
					return true;
				}
				} break;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
HBITMAP icon2bmp(HICON hicon) {
	/* Преобразует иконку hicon в формат точечного рисунка, причем рисунок имеет размеры 18x18, и его фон такой же, как и у пункта меню. */
	HBITMAP hbmp;
	HDC hdcBmp;
	HBRUSH menuBrush;
	MENUITEMINFO minf;        

	/* Создание дексриптора для рисунка. */
	hdcBmp = CreateCompatibleDC(GetDC(NULL));

	/* Создание кисти. */
	menuBrush = CreateSolidBrush(GetSysColor(COLOR_MENU));
	SelectObject(hdcBmp, menuBrush); 

	/* Создание самого рисунка. */
	hbmp = CreateCompatibleBitmap(GetDC(NULL), 18, 18); 
	SelectObject(hdcBmp, hbmp);

	/* Закраска фона цветом меню. */
	ExtFloodFill(hdcBmp, 0, 0, GetSysColor(COLOR_MENU), FLOODFILLBORDER); 

	/* Рисование иконки. */
	SelectObject(hdcBmp, hbmp); 
	DrawIconEx(hdcBmp, 1,1, hicon, 16,16, NULL, CreateSolidBrush( GetSysColor(COLOR_MENU) ), DI_NORMAL | DI_COMPAT);

	/* Очищение памяти. */
	DeleteDC(hdcBmp);

	/* Возвращение рисунка. */
	return hbmp;
}

}