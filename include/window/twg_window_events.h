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
		virtual void onMouse(int32 x, int32 y, MouseType type, int32 wheel) {};
		virtual void onKeyboard(int32 key, bool isDown) {};
		virtual void onSize(Point_i newSize) {};
		virtual void onSizing(int32& left,
							  int32& top,
							  int32& right,
							  int32& bottom,
							  SizingType type) {};
		virtual void onMoving(int32& left,
							  int32& top,
							  int32& right,
							  int32& bottom) {};
		virtual void onActivate(bool isActive, bool isMinimized) {};
		virtual void onComand(int32 comandNo) {};
		virtual void onClose(void) {};
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
	enum TaskbarColor
	{
		TASKBAR_GREEN,
		TASKBAR_YELLOW,
		TASKBAR_RED,
		TASKBAR_LOADING,
		TASKBAR_BLINK
	};

	enum MouseType
	{
		MOUSE_L_DBL, 	MOUSE_L_DOWN, 	MOUSE_L_UP,
		MOUSE_R_DBL, 	MOUSE_R_DOWN, 	MOUSE_R_UP,
		MOUSE_M_DBL, 	MOUSE_M_DOWN, 	MOUSE_M_UP,
		MOUSE_X1_DBL, 	MOUSE_X1_DOWN, 	MOUSE_X1_UP,
		MOUSE_X2_DBL, 	MOUSE_X2_DOWN, 	MOUSE_X2_UP,

		MOUSE_WHEEL, 
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