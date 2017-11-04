#ifndef TWG_CTRL_INCLUDED
#define TWG_CTRL_INCLUDED

#include <vector>
#include <utility>

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
		CtrlBase(EventsBase* parent);

		virtual ~CtrlBase() {}

		virtual void draw(ImageBase* buffer) {}

		int32u 			id;
	protected:
		CtrlStorage* 	m_storage;
	};

	//-------------------------------------------------------------------------
	class CtrlStorage : public CtrlBase
	{
	public:
		CtrlStorage(EventsBase* parent) : CtrlBase(parent), OMFOC(false), m_messageDepth(0) {}
		~CtrlStorage();

		int32u getId(CtrlBase* ctrl);
		int32u getPos(CtrlBase* ctrl);
		void deleteMe(CtrlBase* ctrl);

		std::vector<CtrlBase*>	array;
		bool 					OMFOC; // One Message For One Ctrl

		//---------------------------------------------------------------------
		void draw(ImageBase* buffer);

		bool onMouse(Point_i pos, MouseType type);
		bool onKeyboard(KeyType key, bool isDown);

		bool onResize(Rect rect, SizingType type);
		bool onMove(Point_i newPos);
		bool onFocus(bool isKilled);

		bool onMessage(int32u messageNo, void* data);
		void* sendMessageUp(int32u messageNo, void* data);
	private:
		class IdDistributor {
		public:
			// Если такой указатель есть в массиве, то возвращает его айди, иначе создает новый айди для него
			int32u getId(void* pointer);
			void deleteId(void* pointer);
		private:
			std::vector<std::pair<void*, int32u> > 	m_ids;
			std::vector<int32u> 					m_freeId;
		};

		std::vector<CtrlBase*>	m_toDelete;
		std::vector<CtrlBase*>	m_toAdd;
		IdDistributor			m_distributor;
		int32u 					m_messageDepth;

		void deleteCtrls(void);

		template<class Function>
		bool sendMessage(Function f);
	};

	//-------------------------------------------------------------------------
	enum CtrlMessages : int32u
	{
		CTRL_GET_POINTER = 1002
	};
}

#endif // TWG_CTRL_INCLUDED