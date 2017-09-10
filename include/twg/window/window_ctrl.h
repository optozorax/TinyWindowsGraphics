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
	struct 	onMessageStruct;
	enum 	WindowMessages : int32u;

	/** @ingroup Окна
		@{
	 */

	//-------------------------------------------------------------------------
	/** Имеет хранилище для ctrl'ов, каждый обрабатывает все возможные события.
	   Для плавности рисования имеет второй буфер. Все контролы рисуются 
	   сначала в него, а затем на экран.
	 */
	class WindowCtrl : public WindowEvents
	{
	public:
		WindowCtrl(WindowType type, EventsBase* parent = nullptr) : 
			WindowEvents(type, parent), 
			storage(this), 
			m_buffer(new ImageBase(Point_i(1000, 1000))),
			m_worthRedraw(false) {}

		CtrlStorage 	storage;

		void worthRedraw();
		bool redraw(bool returnType = true);	

		//---------------------------------------------------------------------
		bool onMessage(int32u messageNo, void* data);
		void* sendMessageUp(int32u messageNo, void* data);

		bool onMouse(Point_i pos, MouseType type);
		bool onKeyboard(KeyType key, bool isDown);

		bool onResize(Rect rect, SizingType type);
		bool onMove(Point_i newPos);
		bool onFocus(bool isKilled);
	private:
		ImageBase*		m_buffer;
		bool 			m_worthRedraw;
	};

	//-------------------------------------------------------------------------
	enum WindowMessages : int32u
	{
		WINDOW_GET_POINTER = 1001
	};

	/** @} */
	
}

#endif // TWG_WINDOW_CTRL_INCLUDED