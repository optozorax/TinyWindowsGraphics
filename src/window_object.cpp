#include <map>
#include <windows.h>
#include "twg/window.h"

namespace twg
{

//-----------------------------------------------------------------------------
std::map<HWND, WindowObject*> WindowMap;
LRESULT CALLBACK wndProc1(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	WindowObject *curWin = WindowMap[hwnd];
	if (msg == WM_CREATE) {
		curWin = ((WindowObject*) ((CREATESTRUCT*)lParam)->lpCreateParams);
		WindowMap[hwnd] = curWin;
	}

	if (curWin != nullptr)
		return curWin->wndProc(hwnd, msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------------
bool waitForCreate = false;
DWORD WINAPI makeWindow(LPVOID data) {
	WindowObject* wnd = (WindowObject*)data;
	HWND hwnd = wnd->create((void*)(&wndProc1));

	wnd->m_hwnd = hwnd;
	wnd->m_hdc = GetDC(hwnd);
	wnd->canvas.assign(wnd->m_hdc);

	UpdateWindow(hwnd);
	ShowWindow(hwnd, SW_SHOW);

	waitForCreate = true;

	//-------------------------------------------------------------------------
	MSG msg;
	int32 bRet;

	while((bRet = GetMessage(&msg, hwnd, 0, 0)) != 0) { 
		if (bRet != -1) {
			TranslateMessage(&msg); 
			DispatchMessage(&msg); 
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
void WindowObject::onStart() {
	CreateThread(NULL, 0, &makeWindow, this, 0, NULL);
	while (!waitForCreate) {
		sleep(1);
	}
	waitForCreate = false;
}

//-----------------------------------------------------------------------------
WindowObject::~WindowObject() {
	if (!isClosed())
		DestroyWindow(m_hwnd);
	WindowMap[m_hwnd] = nullptr;
}

//-----------------------------------------------------------------------------
HWND WindowObject::getHwnd(void) {
	return m_hwnd;
}

//-----------------------------------------------------------------------------
void WindowObject::waitForClose(void) {
	while (IsWindow(m_hwnd)) {
		sleep(10);
	}
}

//-----------------------------------------------------------------------------
bool WindowObject::isClosed(void) {
	return IsWindow(m_hwnd);
}

}