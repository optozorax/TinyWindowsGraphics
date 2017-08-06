#include "twg/window/window_events.h"

namespace twg 
{

//-----------------------------------------------------------------------------
LRESULT WindowEvents::wndProcNext(HWND hwnd, 
								  UINT msg,
								  WPARAM wParam, 
								  LPARAM lParam) {
	switch (msg) {
		//---------------------------------------------------------------------
		case WM_COMMAND: {
			onMessageStruct* data = new onMessageStruct(wParam, lParam);
			if (onMessage(int32u(-msg), data))
				return 0;
			} break;

		// case WM_DEADCHAR:
		// case WM_UNICHAR:
		// case WM_KEYDOWN: { 
		// 	wchar_t ws[5] = {};
		// 	BYTE kb[256];
  //   		GetKeyboardState(kb);
		// 	if (ToUnicode(wParam, MapVirtualKey(wParam, MAPVK_VK_TO_VSC), kb, ws, 4, 0) >= 1) 
		// 	    setCaption(getCaption() + std::wstring(ws));
		// 	} break;

		//---------------------------------------------------------------------
		case WM_MOVING: {
			RECT* rect = lParam;
			Point_i pos(rect->left, rect->top);
			if (onMove(pos))
				return TRUE;
			} break;

		//---------------------------------------------------------------------
		case WM_SIZING: {
			SizingType type;
			switch (wParam) {
				case WMSZ_BOTTOM:
					type = SIZING_BOTTOM;
					break;
				case WMSZ_BOTTOMLEFT:
					type = SIZING_BOTTOM_LEFT;
					break;
				case WMSZ_BOTTOMRIGHT:
					type = SIZING_BOTTOM_RIGHT;
					break;
				case WMSZ_LEFT:
					type = SIZING_LEFT;
					break;
				case WMSZ_RIGHT:
					type = SIZING_RIGHT;
					break;
				case WMSZ_TOP:
					type = SIZING_TOP;
					break;
				case WMSZ_TOPLEFT:
					type = SIZING_TOP_LEFT;
					break;
				case WMSZ_TOPRIGHT:
					type = SIZING_TOP_RIGHT;
					break;
			}
			RECT* rect = lParam;
			Point_i newSize(rect->right-rect->left, rect->bottom-rect->top);
			Point_i pos(rect->left, rect->top);
			if (onResize(newSize, pos, type))
				return TRUE;
			} break;

		//---------------------------------------------------------------------
		case WM_KEYUP:
		case WM_KEYDOWN: {
			if (onKeyboard(wParam, msg == WM_KEYDOWN))
				return 0;
			} break;

		//---------------------------------------------------------------------
		case WM_LBUTTONDBLCLK:	case WM_LBUTTONDOWN:	case WM_LBUTTONUP:
		case WM_MBUTTONDBLCLK:	case WM_MBUTTONDOWN:	case WM_MBUTTONUP:
		case WM_RBUTTONDBLCLK:	case WM_RBUTTONDOWN:	case WM_RBUTTONUP: {
			MouseType type = 0;
			switch (msg) {
				case WM_LBUTTONDBLCLK: 	type = MOUSE_L_DBL;		break;
				case WM_LBUTTONDOWN:	type = MOUSE_L_DOWN;	break;
				case WM_LBUTTONUP:		type = MOUSE_L_UP;		break;

				case WM_RBUTTONDBLCLK: 	type = MOUSE_R_DBL;		break;
				case WM_RBUTTONDOWN:	type = MOUSE_R_DOWN;	break;
				case WM_RBUTTONUP:		type = MOUSE_R_UP;		break;

				case WM_MBUTTONDBLCLK: 	type = MOUSE_M_DBL;		break;
				case WM_MBUTTONDOWN:	type = MOUSE_M_DOWN;	break;
				case WM_MBUTTONUP:		type = MOUSE_M_UP;		break;
			}
			Point_i pos(LOWORD(lParam), HIWORD(lParam));
			if (onMouse(pos, type))
				return 0;
			} break;

		//---------------------------------------------------------------------
		case WM_XBUTTONDBLCLK:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP: {
			MouseType type = 0;
			switch (msg) {
				case WM_XBUTTONDBLCLK: 
					if (HIWORD(wParam) == XBUTTON1) type = MOUSE_X1_DBL;
					else							type = MOUSE_X2_DBL;
					break;
				case WM_XBUTTONDOWN:
					if (HIWORD(wParam) == XBUTTON1)	type = MOUSE_X1_DOWN;
					else							type = MOUSE_X2_DOWN;
					break;
				case WM_XBUTTONUP:
					if (HIWORD(wParam) == XBUTTON1)	type = MOUSE_X1_UP;
					else							type = MOUSE_X2_UP;
					break;
			}
			Point_i pos(LOWORD(lParam), HIWORD(lParam));
			if (onMouse(pos, type))
				return TRUE;
			} break;

		//---------------------------------------------------------------------
		case WM_MOUSEMOVE: {
			Point_i pos(LOWORD(lParam), HIWORD(lParam));
			if (onMouse(pos, MOUSE_MOVE))
				return 0;
			} break;

		//---------------------------------------------------------------------
		case WM_MOUSEWHEEL: {
			int32 wheel = GET_WHEEL_DELTA_WPARAM(wParam);
			Point_i pos(LOWORD(lParam), HIWORD(lParam));
			pos = global2client(pos);
			if (onMouse(pos, (wheel > 0) ? MOUSE_WHEEL_UP : MOUSE_WHEEL_DOWN))
				return 0;
			} break;

		//---------------------------------------------------------------------
		case WM_KILLFOCUS: {
			if (onKillFocus())
				return 0;
			} break;		

		//---------------------------------------------------------------------
		case WM_NCHITTEST:
			break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

}