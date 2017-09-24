#include <windows.h>
#include <string>
#include "twg/window.h"

namespace twg
{

//-----------------------------------------------------------------------------
WindowBase::WindowBase(WindowType type) : 
	m_type(new WindowType(type)), 
	m_minSize(type.minSize),
	m_maxSize(type.maxSize) {
	m_isResized = false;
	m_nowRect = nullptr;
	onStart();
	m_initFinish = true;
}

//-----------------------------------------------------------------------------
WindowBase::~WindowBase() {
	UnregisterClass(m_className.c_str(), GetModuleHandle(NULL));
}

//-----------------------------------------------------------------------------
Rect WindowBase::getRect(void) {
	if (m_isResized) 
		return *m_nowRect;
	else {
		RECT rect = {};
		GetWindowRect(m_hwnd, &rect);
		return Rect(rect.left, rect.top, rect.right, rect.bottom);
	}
}

//-----------------------------------------------------------------------------
Point_i WindowBase::getClientSize(void) {
	RECT rect = {};
	GetClientRect(m_hwnd, &rect);
	return Point_i(rect.right, rect.bottom);
}

//-----------------------------------------------------------------------------
Point_i WindowBase::getWindowSize(void) {
	Rect rect = getRect();
	return Point_i(rect.x(), rect.y());
}

//-----------------------------------------------------------------------------
Point_i WindowBase::getPos(void) {
	Rect rect = getRect();
	return Point_i(rect.ax, rect.ay);
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
std::wstring WindowBase::getCaption(void) {
	wchar_t str[100] = {};
	GetWindowText(m_hwnd, str, 100);
	return std::wstring(str);
}

//-----------------------------------------------------------------------------
WindowStyle WindowBase::getStyle(void) {
	DWORD EX_Style = GetWindowLong(m_hwnd, GWL_EXSTYLE);
	DWORD WS_Style = GetWindowLong(m_hwnd, GWL_STYLE);
	int32u style = 0;

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
	else if (EX_Style & WS_EX_LAYERED)
		style |= WINDOW_IMAGE32;
	else
		style |= WINDOW_NO_BORDER;

	return WindowStyle(style);
}

//-----------------------------------------------------------------------------
void WindowBase::setRect(Rect rect) {
	if (m_isResized) 
		*m_nowRect = rect;
	else
		MoveWindow(m_hwnd, rect.ax, rect.ay, rect.x(), rect.y(), TRUE);
}

//-----------------------------------------------------------------------------
void WindowBase::setClientSize(Point_i size) {
	if (!m_isResized) {
		Rect rect = getRect();
		Point_i diff = getWindowSize() - getClientSize();
		Point_i newSize = size + diff;
		rect.bx = rect.ax + newSize.x;
		rect.by = rect.ay + newSize.y;
		setRect(rect);
	}
}

//-----------------------------------------------------------------------------
void WindowBase::setWindowSize(Point_i size) {
	Rect rect = getRect();
	setRect(Rect(rect.ax, rect.ay, rect.ax + size.x, rect.ay + size.y));
}

//-----------------------------------------------------------------------------
void WindowBase::setPos(Point_i pos) {
	Rect rect = getRect();
	setRect(Rect(pos.x, pos.y, pos.x + rect.x(), pos.y + rect.y()));
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
void WindowBase::setCaption(std::wstring caption) {
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
		case WINDOW_IMAGE32:
			WS_Style |= WS_POPUP;
			EX_Style |= WS_EX_LAYERED;
			break;
	}
}

//-----------------------------------------------------------------------------
void WindowBase::setStyle(WindowStyle style) {
	DWORD EX_Style, WS_Style;
	getEX_WS_Styles(style, EX_Style, WS_Style);
	SetWindowLong(m_hwnd, GWL_EXSTYLE, EX_Style);
	SetWindowLong(m_hwnd, GWL_STYLE, WS_Style);
}

//-----------------------------------------------------------------------------
Point_i WindowBase::global2client(Point_i globalPos) {
	POINT p = { globalPos.x, globalPos.y };
	ScreenToClient(m_hwnd, &p);
	return Point_i(p.x, p.y);
}

//-----------------------------------------------------------------------------
Point_i WindowBase::client2global(Point_i clientPos) {
	POINT p = { clientPos.x, clientPos.y };
	ClientToScreen(m_hwnd, &p);
	return Point_i(p.x, p.y);
}

//-----------------------------------------------------------------------------
void WindowBase::invalidateScreen(void) {
	InvalidateRect(m_hwnd, NULL, TRUE);
}

//-----------------------------------------------------------------------------
bool WindowBase::updateImage(ImageBase& img) {
	if (getStyle() % WINDOW_MAX_TYPE == WINDOW_IMAGE32) {
		POINT pptDest = {0, 0};
		Point_i size(getWindowSize());
		size.x = TWG_min(size.x, img.width());
		size.y = TWG_min(size.y, img.height());
		SIZE client = {size.x, size.y};
		BLENDFUNCTION blendFn = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
		UpdateLayeredWindow(m_hwnd, GetDC(m_hwnd), NULL, &client,
			img.getHdc(), &pptDest, 0, &blendFn, ULW_ALPHA);
		return true;
	}
	return false;
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
	wc.lpfnWndProc = (WNDPROC)data;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = CreateSolidBrush(RGB(255,255,255));
	wc.lpszMenuName = NULL;

	if (m_type->iconNo >= ICON_APPLICATION && m_type->iconNo <= ICON_INFORMATION) {
		wc.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(m_type->iconNo));
		wc.hIconSm = LoadIcon(NULL, MAKEINTRESOURCE(m_type->iconNo));
	} else {
		wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(m_type->iconNo));
		wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(m_type->iconNo));
	}

	if (m_type->style & WINDOW_DROP_SHADOW)
		wc.style |= CS_DROPSHADOW;

	//-------------------------------------------------------------------------
	/* Для каждого окна классы уникальны. Регистрируется класс с уникальным именем. */
	long currentClassN = 0;
	bool registered = false;
	do {
		currentClassN++;
		std::string str = std::to_string(currentClassN);
		m_className = L"TWG_CLASS_" + std::wstring(str.begin(), str.end());
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
		//---------------------------------------------------------------------
		case WM_GETMINMAXINFO:
			if (m_minSize != Point_i(-1, -1) && m_maxSize != Point_i(-1, -1)) {
				MINMAXINFO *pInfo = (MINMAXINFO *)(lParam);
				// Установка в переменные минимальных и максимальных размеров стандартных значений
				if (pInfo->ptMinTrackSize.x > m_minSize.x)
					m_minSize.x = pInfo->ptMinTrackSize.x;
				if (pInfo->ptMinTrackSize.y > m_minSize.y)
					m_minSize.y = pInfo->ptMinTrackSize.y;
				pInfo->ptMinTrackSize = { m_minSize.x, m_minSize.y };
				pInfo->ptMaxTrackSize = { m_maxSize.x, m_maxSize.y };
				return 0;
			} break;

		//---------------------------------------------------------------------
		case WM_SIZING: {
			m_nowRect = (Rect*)(lParam);
			m_isResized = true;
			} break;

		//---------------------------------------------------------------------
		case WM_MOVING: {
			m_nowRect = (Rect*)(lParam);
			m_isResized = true;
			} break;
	}

	LRESULT returned;
	if (m_initFinish)
		returned = wndProcNext(hwnd, msg, wParam, lParam);
	else
		returned = DefWindowProc(hwnd, msg, wParam, lParam);

	m_isResized = false;

	return returned;
}

//-----------------------------------------------------------------------------
LRESULT WindowBase::wndProcNext(HWND hwnd, 
								UINT msg,
								WPARAM wParam, 
								LPARAM lParam) {
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

}