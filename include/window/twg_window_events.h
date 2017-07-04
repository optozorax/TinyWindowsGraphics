#ifndef TWG_WINDOW_EVENTS_INCLUDED
#define TWG_WINDOW_EVENTS_INCLUDED

#include "../window.h"
#include "../twg_basics.h"
#include "../twg_point.h"

namespace twg
{

	enum 	MouseType;
	enum 	SizingType;
	enum 	TaskbarColor;

	//-------------------------------------------------------------------------
	// Имеет обработку событий окна.
	class WindowEvents : WindowBase
	{
	public:
		virtual void onMouse(int32 x, int32 y, MouseType type) {};
		virtual void onKeyboard(int32 key, bool isDown) {};
		virtual void onResize(Point_i diffSize, 
							  Point_i diffPos, 
							  SizingType type) {};
		virtual void onMove(Point_i diffPos) {};
		virtual void onMessage(int32 messageNo) {};
		virtual void onKillFocus(void) {};
		virtual void onActivate(void) {};
	protected:
		LRESULT wndProcNext(HWND hwnd, 
							UINT msg,
							WPARAM wParam, 
							LPARAM lParam);

		// По умолчанию возвращает DefWindowProc(hwnd, msg, wParam, lParam);
		virtual LRESULT wndProcNextNext(HWND hwnd, 
										UINT msg,
										WPARAM wParam, 
										LPARAM lParam);
	};

	//-------------------------------------------------------------------------
	enum MouseType
	{
		MOUSE_L_DBL, 	MOUSE_L_DOWN, 	MOUSE_L_UP,
		MOUSE_R_DBL, 	MOUSE_R_DOWN, 	MOUSE_R_UP,
		MOUSE_M_DBL, 	MOUSE_M_DOWN, 	MOUSE_M_UP,
		MOUSE_X1_DBL, 	MOUSE_X1_DOWN, 	MOUSE_X1_UP,
		MOUSE_X2_DBL, 	MOUSE_X2_DOWN, 	MOUSE_X2_UP,

		MOUSE_WHEEL_UP,
		MOUSE_WHEEL_DOWN,

		MOUSE_MOVE
	};

	enum SizingType
	{
		SIZING_LEFT,
		SIZING_RIGHT,
		SIZING_BOTTOM,
		SIZING_TOF,

		SIZING_BOTTOM_LEFT,
		SIZING_BOTTOM_RIGHT,
		SIZING_TOP_LEFT,
		SIZING_TOP_RIGHT
	};
	
}

#endif // TWG_WINDOW_EVENTS_INCLUDED