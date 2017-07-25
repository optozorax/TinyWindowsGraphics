#include <map>
#include <windows.h>
#include "twg_window.h"

std::map<HWND, window*> WindowMap;
LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	WindowBase *curWin = WindowMap[hwnd];
	if (msg == WM_CREATE) {
		WindowBase *curWin = ((WindowBase*) 
			((CREATESTRUCT*)lParam)->lpCreateParams);
		WindowMap[hwnd] = curWin;
	}

	if (curWin != 0)
		return curWin->wndProc(hwnd, msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}

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

WindowBase::WindowBase() 
{
	m_hwnd = create((void*)(wndProc));
	m_hdc = GetDC(m_hwnd);

	UpdateWindow(hwnd);
	ShowWindow(hwnd, SW_SHOW);

	CreateThread(NULL, 0, &msgCycle, hwnd, 0, NULL);
}