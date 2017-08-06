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
	enum CtrlMessages : int32u
	{
		CTRL_GET_POINTER = 1001,
		CTRL_GET_UNIQUE_ID = 1002
	};

	//-------------------------------------------------------------------------
	class CtrlBase : public EventsHandler
	{
	public:
		CtrlBase(EventsBase* parent) : 
			EventsHandler(parent) {
			int32u* idp = sendMessageUp(CTRL_GET_UNIQUE_ID, nullptr);
			id = *idp;
			delete idp;
		}

		virtual ~CtrlBase() {}

		virtual void draw(ImageBase* buffer) {}
	public:
		int32u	id;
	};

	//-------------------------------------------------------------------------
	class CtrlStorage : CtrlBase
	{
	public:
		CtrlStorage(EventsBase* parent) : 
			CtrlBase(m_parent) {}
		~CtrlStorage() {}

		int32u getUniqueId(void);
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
		std::vector<int32u> 	m_ids;
	};

}

#endif // TWG_CTRL_INCLUDED