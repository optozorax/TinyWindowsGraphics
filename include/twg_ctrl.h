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
		virtual CtrlBase() {};
		virtual ~CtrlBase() {};

		virtual void onMouse(int32 x, int32 y, MouseType type, int32 wheel) {};
		virtual void onKeyboard(int32 key, bool isDown) {};
		virtual void onSize(Point_i newSize) {};
		virtual void onSizing(int32& left,
							  int32& top,
							  int32& right,
							  int32& bottom,
							  SizingType type) {};
		virtual void onMoving(int32& left,
							  int32& top,
							  int32& right,
							  int32& bottom) {};
		virtual void onActivate(bool isActive, bool isMinimized) {};
		virtual void onComand(int32 comandNo) {};
		virtual void onClose(void) {};

		virtual void drawOn(ImageBase *img) {};

		int32u		id;
		bool 		isDraw;
		bool 		isRunning;
	protected:
		WindowBase*	m_window;
	};

	//-------------------------------------------------------------------------
	class Menu : CtrlBase
	{
		createMenu(std::string menuStr);

		/** @todo Подумать как можно реализовать этот интерфейс.
		 */

		virtual void onClick(int32 comand);
	private:
		void onClick(int32 comandNo);
		void drawOn(ImageBase *img) {};
	};

	//-------------------------------------------------------------------------
	enum MenuLineType
	{
		MENU_LINE 			= 0,
		MENU_POPUP 			= 1,
		MENU_SEPARATOR 		= 2,
		MENU_CHECK_IMAGE 	= 3,

		MENU_CHECKED 		= 0x04
		MENU_GRAYED 		= 0x08,
		MENU_RIGHT_ALIGN 	= 0x10
	};

}

#endif // TWG_CTRL_INCLUDED