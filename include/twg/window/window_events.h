#ifndef TWG_WINDOW_EVENTS_INCLUDED
#define TWG_WINDOW_EVENTS_INCLUDED

#include "twg/events.h"
#include "twg/ctrl.h"
#include "twg/window.h"
#include "twg/basics.h"
#include "twg/point.h"

namespace twg
{

	class WindowEvents;
	struct onMessageStruct;
	enum WindowsMessages : int32u;

	//-------------------------------------------------------------------------
	class WindowEvents : public WindowBase, public CtrlBase
	{
	public:
		WindowEvents(EventsBase* parent = nullptr) : CtrlBase(parent) {}
		WindowEvents(WindowType type, EventsBase* parent = nullptr) : 
			WindowBase(type), 
			CtrlBase(parent) {}
	protected:
		LRESULT wndProcNext(HWND hwnd, 
							UINT msg,
							WPARAM wParam, 
							LPARAM lParam);
	};

	//-------------------------------------------------------------------------
	struct onMessageStruct 
	{
		onMessageStruct(HWND hwnd1, UINT msg1, WPARAM wParam1, LPARAM lParam1, LRESULT lResult1 = 0) {
			hwnd = hwnd1;
			msg = msg1;
			wParam = wParam1;
			lParam = lParam1;
			lResult = lResult1;
		}

		HWND 	hwnd;
		UINT 	msg;
		WPARAM 	wParam;
		LPARAM 	lParam;
		LRESULT lResult;
	};

	//-------------------------------------------------------------------------
	enum WindowsMessages : int32u {
		WINDOWS_MESSAGE = 1000
	};
	
}

#endif // TWG_WINDOW_EVENTS_INCLUDED