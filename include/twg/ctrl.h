#ifndef TWG_CTRL_INCLUDED
#define TWG_CTRL_INCLUDED

#include <vector>

#include "twg/basics.h"
#include "twg/point.h"
#include "twg/events.h"
#include "twg/image.h"

namespace twg
{

	enum 	CtrlMessages : int32u;
	class 	CtrlBase;
	class 	CtrlStorage;

	//-------------------------------------------------------------------------
	class CtrlBase : public EventsHandler
	{
	public:
		CtrlBase(EventsBase* parent) : EventsHandler(parent) { }

		virtual ~CtrlBase() {}

		virtual void draw(ImageBase* buffer) {}
	};

	//-------------------------------------------------------------------------
	class CtrlStorage : public CtrlBase
	{
	public:
		CtrlStorage(EventsBase* parent) : CtrlBase(parent), OMFOC(false) {}
		~CtrlStorage() {}

		int32u getId(CtrlBase* ctrl);
		void deleteMe(CtrlBase* ctrl);

		std::vector<CtrlBase*>	array;
		bool 					OMFOC; // One Message For One Ctrl

		//---------------------------------------------------------------------
		void draw(ImageBase* buffer);

		bool onMouse(Point_i pos, MouseType type);
		bool onKeyboard(KeyType key, bool isDown);

		bool onResize(Point_i newSize, Point_i pos, SizingType type);
		bool onMove(Point_i newPos);
		bool onKillFocus(void);

		bool onMessage(int32u messageNo, void* data);
		void* sendMessageUp(int32u messageNo, void* data);
	private:
		std::vector<CtrlBase*>	m_toDelete;

		void deleteCtrls(void);
	};

	//-------------------------------------------------------------------------
	enum CtrlMessages : int32u
	{
		CTRL_GET_POINTER = 1002
	};
}

#endif // TWG_CTRL_INCLUDED