#ifndef TWG_WINDOW_CTRL_INCLUDED
#define TWG_WINDOW_CTRL_INCLUDED

#include <string>

#include "twg_window_events.h"
#include "../twg_ctrl.h"
#include "../twg_window.h"
#include "../twg_basics.h"

namespace twg
{

	/* Имеет хранилище для ctrl'ов, каждый обрабатывает все возможные события.
	   Для плавности рисования имеет второй буфер. Все контролы рисуются 
	   сначала в него, а затем на экран.
	 */
	class WindowCtrl : WindowEventsBase
	{
	public:
		addCtrl(Ctrl* ctrl);

		/** @todo Чтобы можно было настраивать порядок, удалять ctrl'ы.
			@todo Сделать контролы на стандартное меню, кнопку, textEdit.
			@todo Реализовать с учетом возможной анимации ctrl'ов.
		 */
	private:
		void onMouse(int32 x, int32 y, MouseType type, int32 wheel) {};
		void onKeyboard(int32 key, bool isDown) {};
		void onSize(Point_i newSize) {};
		void onSizing(int32& left,
					  int32& top,
					  int32& right,
					  int32& bottom,
					  SizingType type) {};
		void onMoving(int32& left,
					  int32& top,
					  int32& right,
					  int32& bottom) {};
		void onActivate(bool isActive, bool isMinimized) {};
		void onComand(int32 comandNo) {};
		void onClose(void) {};
	};

	
}

#endif // TWG_WINDOW_CTRL_INCLUDED