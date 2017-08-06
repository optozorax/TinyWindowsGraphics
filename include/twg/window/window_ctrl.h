#ifndef TWG_WINDOW_CTRL_INCLUDED
#define TWG_WINDOW_CTRL_INCLUDED

#include <string>

#include "twg/window/window_events.h"
#include "twg/ctrl.h"
#include "twg/window.h"
#include "twg/basics.h"

namespace twg
{

	class 	WindowCtrl;
	enum 	WindowMessages : int32u;

	//-------------------------------------------------------------------------
	/* Имеет хранилище для ctrl'ов, каждый обрабатывает все возможные события.
	   Для плавности рисования имеет второй буфер. Все контролы рисуются 
	   сначала в него, а затем на экран.
	 */
	class WindowCtrl : public WindowEvents
	{
	public:
		WindowCtrl(WindowType type) : 
			WindowEvents(type), 
			storage(this), 
			m_buffer(new ImageBase(Point_i(2000, 2000))) {}

		CtrlStorage 	storage;	

		//---------------------------------------------------------------------
		bool onMessage(int32u messageNo, void* data);
		// Здесь обрабатывается сообщение WINDOW_GET_HWND, а все остальное отправляется на onMessage хранилищу.
		void* sendMessageUp(int32u messageNo, void* data);

		bool onMouse(Point_i pos, MouseType type);
		bool onKeyboard(KeyType key, bool isDown);

		bool onResize(Point_i newSize, 
					  Point_i pos, 
					  SizingType type);
		bool onMove(Point_i newPos);
		bool onKillFocus(void);
	private:
		// В resize следить за размером окна, потому что может понадобиться ресайзить буфер.
		ImageBase*		m_buffer;
	};

	//-------------------------------------------------------------------------
	enum WindowMessages : int32u
	{
		WINDOW_GET_POINTER = 1000
	};
	
}

#endif // TWG_WINDOW_CTRL_INCLUDED