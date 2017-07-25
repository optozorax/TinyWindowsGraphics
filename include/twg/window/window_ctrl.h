#ifndef TWG_WINDOW_CTRL_INCLUDED
#define TWG_WINDOW_CTRL_INCLUDED

#include <string>

#include "twg/window/window_events.h"
#include "twg/ctrl.h"
#include "twg/window.h"
#include "twg/basics.h"

namespace twg
{

	/* Имеет хранилище для ctrl'ов, каждый обрабатывает все возможные события.
	   Для плавности рисования имеет второй буфер. Все контролы рисуются 
	   сначала в него, а затем на экран.
	 */
	class WindowCtrl : WindowEventsBase
	{
	public:
		int32u addCtrl(Ctrl* ctrl);
		void deleteCtrl(int32u id);
		void makeOrder(int32u idMove2Up, int32u idMove2Down);
		
		int32u getUniqueId(void);
		int32u getCtrlNum(void);

		void runTimer(int32u delay);
		void stopTimer(void);
	private:
		
	};

	
}

#endif // TWG_WINDOW_CTRL_INCLUDED