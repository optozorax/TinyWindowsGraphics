#include <windows.h>
#include "twg/window.h"

namespace twg
{

//-----------------------------------------------------------------------------
WindowBase::WindowBase(WindowType type) : 
	m_type(new WindowType(type)), 
	m_minSize(type.minSize),
	m_maxSize(type.maxSize) {
	onStart();
}

//-----------------------------------------------------------------------------
WindowBase::~WindowBase() {
	UnregisterClass(m_className.c_str(), GetModuleHandle(NULL));
}

//-----------------------------------------------------------------------------
Point_i WindowBase::getClientSize(void) {
	RECT rc = {};
	GetClientRect(m_hwnd, &rc);
	return Point_i(rc.right, rc.bottom);
}

//-----------------------------------------------------------------------------
Point_i WindowBase::getWindowSize(void) {
	RECT rc = {};
	GetWindowRect(m_hwnd, &rc);
	return Point_i(rc.right-rc.left, rc.bottom-rc.top);
}

//-----------------------------------------------------------------------------
Point_i WindowBase::getPos(void) {
	RECT rc = {};
	GetWindowRect(m_hwnd, &rc);
	return Point_i(rc.left, rc.top);
}

//-----------------------------------------------------------------------------
Point_i WindowBase::getMaxSize(void) {
	return m_maxSize;
}

//-----------------------------------------------------------------------------
Point_i WindowBase::getMinSize(void) {
	return m_minSize;
}

//-----------------------------------------------------------------------------
std::string WindowBase::getCaption(void) {
	char str[100] = {};
	GetWindowText(m_hwnd, str, 100);
	return std::string(str);
}

//-----------------------------------------------------------------------------
WindowStyle WindowBase::getStyle(void) {
	DWORD EX_Style = GetWindowLong(m_hwnd, GWL_EXSTYLE);
	DWORD WS_Style = GetWindowLong(m_hwnd, GWL_STYLE);
	WindowStyle style = 0;

	if (EX_Style & WS_EX_TOPMOST)	style |= WINDOW_ON_TOP;
	if (WS_Style & WS_DISABLED)		style |= WINDOW_DISABLED;
	if (WS_Style & WS_MAXIMIZE)		style |= WINDOW_MAXIMIZED;
	if (WS_Style & WS_MAXIMIZEBOX)	style |= WINDOW_MAX_BUTTON;
	if (WS_Style & WS_MINIMIZEBOX)	style |= WINDOW_MIN_BUTTON;

	if (WS_Style & WS_TILEDWINDOW)
		if (EX_Style & WS_EX_TOOLWINDOW)
			style |= WINDOW_TOOL;
		else
			style |= WINDOW_STANDART;
	else if (WS_Style & WS_THICKFRAME)
		style |= WINDOW_POPUP;
	else if (EX_Style & WS_EX_TOOLWINDOW)
		style |= WINDOW_CAPTION;
	else
		style |= WINDOW_NO_BORDER;

	return style;
}

//-----------------------------------------------------------------------------
void WindowBase::setClientSize(Point_i size) {
	Point_i pos = getPos();
	Point_i diff = getWindowSize() - getClientSize();
	Point_i newSize = size + diff;
	MoveWindow(m_hwnd, pos.x, pos.y, newSize.x, newSize.y, TRUE);
}

//-----------------------------------------------------------------------------
void WindowBase::setWindowSize(Point_i size) {
	Point_i pos = getPos();
	MoveWindow(m_hwnd, pos.x, pos.y, size.x, size.y, TRUE);
}

//-----------------------------------------------------------------------------
void WindowBase::setPos(Point_i pos) {
	Point_i size = getWindowSize();
	MoveWindow(m_hwnd, pos.x, pos.y, size.x, size.y, TRUE);	
}

//-----------------------------------------------------------------------------
void WindowBase::setMaxSize(Point_i maxSize) {
	m_maxSize = maxSize;
}

//-----------------------------------------------------------------------------
void WindowBase::setMinSize(Point_i minSize) {
	m_minSize = minSize;
}

//-----------------------------------------------------------------------------
void WindowBase::setCaption(std::string caption) {
	SetWindowText(m_hwnd, caption.c_str());
}

//-----------------------------------------------------------------------------
void getEX_WS_Styles(WindowStyle style, DWORD& EX_Style, DWORD& WS_Style) {
	EX_Style = 0;
	WS_Style = 0;
	if (style & WINDOW_ON_TOP)		EX_Style |= WS_EX_TOPMOST;
	if (style & WINDOW_DISABLED)	WS_Style |= WS_DISABLED;
	if (style & WINDOW_MAXIMIZED)	WS_Style |= WS_MAXIMIZE;
	if (style & WINDOW_MAX_BUTTON)	WS_Style |= WS_MAXIMIZEBOX;
	if (style & WINDOW_MIN_BUTTON)	WS_Style |= WS_MINIMIZEBOX;

	switch (style % WINDOW_MAX_TYPE) {
		case WINDOW_STANDART:
			WS_Style |= WS_TILEDWINDOW;
			break;
		case WINDOW_TOOL:
			WS_Style |= WS_TILEDWINDOW;
			EX_Style |= WS_EX_TOOLWINDOW;
			break;
		case WINDOW_CAPTION:
			WS_Style |= WS_POPUP | WS_CAPTION;
			EX_Style |= WS_EX_TOOLWINDOW;
			break;
		case WINDOW_POPUP:
			WS_Style |= WS_POPUP | WS_THICKFRAME;
			break;
		case WINDOW_NO_BORDER:
			WS_Style |= WS_POPUP;
			break;
	}
}

void WindowBase::setStyle(WindowStyle style) {
	DWORD EX_Style, WS_Style;
	getEX_WS_Styles(style, EX_Style, WS_Style);
	SetWindowLong(m_hwnd, GWL_EXSTYLE, EX_Style);
	SetWindowLong(m_hwnd, GWL_STYLE, WS_Style);
}

//-----------------------------------------------------------------------------
void WindowBase::invalidateScreen(void) {
	InvalidateRect(m_hwnd, NULL, TRUE);
}

//-----------------------------------------------------------------------------
HWND WindowBase::create(void* data) {
	HINSTANCE hInstance = GetModuleHandle(NULL);

	//-------------------------------------------------------------------------
	/* Создание переменной для регистрации. */
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | 
			   CS_VREDRAW | 
			   CS_PARENTDC | 
			   CS_SAVEBITS | 
			   CS_DBLCLKS;
	wc.lpfnWndProc = data;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = CreateSolidBrush(RGB(255,255,255));
	wc.lpszMenuName = NULL;

	if (m_type->iconNo == -1)
		wc.hIcon = NULL;
	else
		wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(m_type->iconNo));
	wc.hIconSm = wc.hIcon;

	if (m_type->style & WINDOW_DROP_SHADOW)
		wc.style |= CS_DROPSHADOW;

	//-------------------------------------------------------------------------
	/* Для каждого окна классы уникальны. Регистрируется класс с уникальным именем. */
	long currentClassN = 0;
	bool registered = false;
	do {
		currentClassN++;
		m_className = "TWG_CLASS_" + std::to_string(currentClassN);
		wc.lpszClassName = m_className.c_str();
		registered = RegisterClassEx(&wc) != 0;
	} while (!registered);

	//-------------------------------------------------------------------------
	/* Заполнение переменных стиля. */
	DWORD EX_Style, WS_Style;
	getEX_WS_Styles(m_type->style, EX_Style, WS_Style);

	//-------------------------------------------------------------------------
	/* Создание окна. */
	HWND hwnd = CreateWindowEx(EX_Style,
							   m_className.c_str(),
							   m_type->caption.c_str(),
							   WS_Style,
							   m_type->pos.x,  m_type->pos.y,
							   m_type->size.x, m_type->size.y,
							   NULL, NULL, hInstance, 
							   this);
	return hwnd;
}

//-----------------------------------------------------------------------------
LRESULT WindowBase::wndProc(HWND hwnd, 
							UINT msg,
							WPARAM wParam, 
							LPARAM lParam) {
	switch (msg) {
		case WM_GETMINMAXINFO:
		if (m_minSize != Point_i(-1, -1) && m_maxSize != Point_i(-1, -1)) {
			MINMAXINFO *pInfo = (MINMAXINFO *)(lParam);
			Point_i diff = getWindowSize() - getClientSize();
			pInfo->ptMinTrackSize = { m_minSize.x+diff.x, m_minSize.y+diff.y };
			pInfo->ptMaxTrackSize = { m_maxSize.x+diff.x, m_maxSize.y+diff.y };
			return 0;
		} break;
	}

	return wndProcNext(hwnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------------
LRESULT WindowBase::wndProcNext(HWND hwnd, 
								UINT msg,
								WPARAM wParam, 
								LPARAM lParam) {
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

}