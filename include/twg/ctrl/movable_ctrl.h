#ifndef TWG_MOVABLE_CTRL_INCLUDED
#define TWG_MOVABLE_CTRL_INCLUDED

#include <string>
#include "twg/events.h"
#include "twg/ctrl.h"
#include <twg/window/window_ctrl.h>

namespace twg
{

	class MovableCtrl;

	//-------------------------------------------------------------------------
	class MovableCtrl : public CtrlBase 
	{
	public:
		MovableCtrl(EventsBase* parent);

		//---------------------------------------------------------------------
		bool onMouse(Point_i pos, MouseType type) final;
		bool onKeyboard(KeyType key, bool isDown) final;
		void draw(ImageBase* buffer) final;
	protected:
		virtual bool isInside(Point_i pos) = 0;
		virtual void move(Point_i diff) = 0;

		virtual bool onRMouse(Point_i pos) { return false; };
		virtual bool onWheel(Point_i pos, MouseType wheel) { return false; };

		virtual void drawDefault(ImageBase* buffer) {};
		virtual void drawHover(ImageBase* buffer) { drawDefault(buffer); };
		virtual void drawWhenMove(ImageBase* buffer) { drawDefault(buffer); };

	private:
		enum MyState
		{
			STATE_DEFAULT,
			STATE_MOVE,
			STATE_HOVER
		};

		MyState			m_current;
		Point_i 		m_lastPos;
		WindowCtrl*		m_wnd;
	};

}

#endif // TWG_MOVABLE_CTRL_INCLUDED