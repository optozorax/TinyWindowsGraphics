#ifndef TWG_BRAIN_CTRL_INCLUDED
#define TWG_BRAIN_CTRL_INCLUDED

#include <string>
#include <twg/events.h>
#include <twg/ctrl.h>
#include <twg/window/window_ctrl.h>

namespace twg
{

	class BrainCtrl;

	//-------------------------------------------------------------------------
	class BrainCtrl : public CtrlBase 
	{
	public:
		BrainCtrl(EventsBase* parent);

		virtual void init(void) {}
		virtual bool onMessageNext(int32u messageNo, void* data) { return false; }

		//---------------------------------------------------------------------
		bool onMessage(int32u messageNo, void* data) final;
	protected:
		WindowCtrl*		m_wnd;
	private:
		bool			m_initialized;
	};

}

#endif // TWG_MOVABLE_CTRL_INCLUDED