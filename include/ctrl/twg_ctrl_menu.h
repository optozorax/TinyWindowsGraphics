#ifndef TWG_CTRL_MENU_INCLUDED
#define TWG_CTRL_MENU_INCLUDED

#include <string>
#include "../twg_ctrl.h"

namespace twg
{

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

#endif // TWG_CTRL_MENU_INCLUDED