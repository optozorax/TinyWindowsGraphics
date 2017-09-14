#ifndef TWG_CLICKABLE_CTRL_INCLUDED
#define TWG_CLICKABLE_CTRL_INCLUDED

#include <string>
#include "twg/events.h"
#include "twg/ctrl.h"
#include <twg/window/window_ctrl.h>

namespace twg
{

	class ClickableCtrl;

	//-------------------------------------------------------------------------
	class ClickableCtrl : public CtrlBase 
	{
	public:
		ClickableCtrl(EventsBase* parent);

		//---------------------------------------------------------------------
		bool onMouse(Point_i pos, MouseType type) final;
		bool onFocus(bool isKilled) final;
		void draw(ImageBase* buffer) final;
	protected:
		virtual bool isInside(Point_i pos) = 0;
		virtual void onClick(void) = 0;

		virtual bool onRMouse(Point_i pos) { return false; };
		virtual bool onWheel(Point_i pos, MouseType wheel) { return false; };

		virtual void drawDefault(ImageBase* buffer) {};
		virtual void drawHover(ImageBase* buffer) { drawDefault(buffer); };
		virtual void drawWhenClick(ImageBase* buffer) { drawDefault(buffer); };

		WindowCtrl*		m_wnd;
	private:
		enum MyState
		{
			STATE_DEFAULT,
			STATE_CLICK,
			STATE_HOVER
		};

		MyState			m_current;
	};

}

#endif // TWG_MOVABLE_CTRL_INCLUDED