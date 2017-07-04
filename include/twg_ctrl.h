#ifndef TWG_CTRL_INCLUDED
#define TWG_CTRL_INCLUDED

#include <string>

#include "twg_basics.h"
#include "twg_point.h"
#include "twg_window.h"

namespace twg
{

	class CtrlBase
	{
	public:
		virtual void onMouse(Point_i pos, MouseType type) {};
		virtual void onKeyboard(int32 key, bool isDown) {};
		virtual void onResize(Point_i diffSize, 
							  Point_i diffPos, 
							  SizingType type) {};
		virtual void onMove(Point_i newPos) {};
		virtual void onMessage(int32 messageNo) {};
		virtual void onTimer(void) {};
		virtual void onKillFocus(void) {};

		virtual void draw(void) {};

		int32u		id;
		bool 		isEnabled; /**< Если включен, то принимает все сообщения, 
							   		иначе только рисуется по-особому. */
	protected:
		void*		m_parent; ///< Окно, которому принадлежит данный ctrl
		ImageBase* 	m_bufer; ///< Сюда рисуется ctrl при вызове draw
	};

}

#endif // TWG_CTRL_INCLUDED