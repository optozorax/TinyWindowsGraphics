#include <map>
#include <windows.h>
#include "twg/window.h"

namespace twg
{

//-----------------------------------------------------------------------------
std::map<HWND, WindowObject*> WindowMap;
LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	WindowObject *curWin = WindowMap[hwnd];
	if (msg == WM_CREATE) {
		WindowObject *curWin = ((WindowObject*) 
			((CREATESTRUCT*)lParam)->lpCreateParams);
		WindowMap[hwnd] = curWin;
	}

	if (curWin != 0)
		return curWin->wndProc(hwnd, msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------------
DWORD WINAPI msgCycle(LPVOID hwnd)
{
	MSG msg;
	bool bRet;

	while((bRet = GetMessage(&msg, (HWND)hwnd, 0, 0)) != 0) { 
		if (bRet != -1) {
			TranslateMessage(&msg); 
			DispatchMessage(&msg); 
		}
	}
}

//-----------------------------------------------------------------------------
WindowObject::WindowObject() 
{
	m_hwnd = create((void*)(&wndProc));
	m_hdc = GetDC(m_hwnd);

	UpdateWindow(m_hwnd);
	ShowWindow(m_hwnd, SW_SHOW);

	CreateThread(NULL, 0, &msgCycle, m_hwnd, 0, NULL);
}

}