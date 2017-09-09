#ifndef TWG_EVENTS_INCLUDED
#define TWG_EVENTS_INCLUDED

#include <string>

#include "twg/basics.h"
#include "twg/point.h"

namespace twg
{

	class 	EventsBase;
	class 	EventsHandler;
	enum 	MouseType : int32u;
	enum 	SizingType : int32u;

	//-------------------------------------------------------------------------
	class EventsBase 
	{
	public:
		EventsBase(EventsBase* parent) : m_parent(parent) {}
		virtual ~EventsBase() {}

		/** Получение сообщения от предка, если это контрол. 
			Если это хранилище, то после каждого полученного сообщения в onMessage необходимо вызывать с тем же самым сообщением всех потомков.
		 */
		virtual bool onMessage(int32u messageNo, void* data) { return false; }

		/** Сообщение, пришедшее от потомка, которое надо отправить наверх
			в зависимости от обстоятельств. Здесь же можно обработать это 
			сообщение, как способ потомка общаться с родителем. По умолчанию 
			отправляется наверх. Если это возможно, то сразу возвращает необходимое значение без отправления сообщения вниз.
			Когда возвращается сразу необходимое значение, то обязательно надо после обработки освобождать память. А вот, отправляя вверх.
		 */
		virtual void* sendMessageUp(int32u messageNo, void* data) {
			if (m_parent != nullptr)
				return m_parent->sendMessageUp(messageNo, data);
			else {
				onMessage(messageNo, data);
				return nullptr;
			}
		}
	protected:
		EventsBase*	m_parent;
	};

	//-------------------------------------------------------------------------
	class EventsHandler : public EventsBase
	{
	public:
		EventsHandler(EventsBase* parent) : EventsBase(parent) {}
		virtual ~EventsHandler() {}

		virtual bool onMouse(Point_i pos, MouseType type) { return false; }
		virtual bool onKeyboard(KeyType key, bool isDown) { return false; }

		virtual bool onResize(Rect rect, SizingType type) { return false; }

		// @TODO переделать, чтобы было на рект
		virtual bool onMove(Point_i newPos) { return false; }
		virtual bool onFocus(bool isKilled) { return false; }
	};

	//-------------------------------------------------------------------------
	enum MouseType : int32u
	{
		MOUSE_L_DBL, 	MOUSE_L_DOWN, 	MOUSE_L_UP,
		MOUSE_R_DBL, 	MOUSE_R_DOWN, 	MOUSE_R_UP,
		MOUSE_M_DBL, 	MOUSE_M_DOWN, 	MOUSE_M_UP,
		MOUSE_X1_DBL, 	MOUSE_X1_DOWN, 	MOUSE_X1_UP,
		MOUSE_X2_DBL, 	MOUSE_X2_DOWN, 	MOUSE_X2_UP,

		MOUSE_WHEEL_UP,
		MOUSE_WHEEL_DOWN,

		MOUSE_MOVE
	};

	enum SizingType : int32u
	{
		SIZING_LEFT,
		SIZING_RIGHT,
		SIZING_BOTTOM,
		SIZING_TOP,

		SIZING_BOTTOM_LEFT,
		SIZING_BOTTOM_RIGHT,
		SIZING_TOP_LEFT,
		SIZING_TOP_RIGHT,

		SIZING_MAXIMIZED,
		SIZING_MINIMIZED,
		SIZING_RESTORED
	};

}

#endif // TWG_CTRL_INCLUDED