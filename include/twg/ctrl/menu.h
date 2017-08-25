#ifndef TWG_CTRL_MENU_INCLUDED
#define TWG_CTRL_MENU_INCLUDED

#include <string>
#include "twg/events.h"
#include "twg/ctrl.h"

namespace twg
{

	class StaticMenu;
	class PopupMenu;
	enum MenuMessages : int32u;

	//-------------------------------------------------------------------------
	class StaticMenu : public CtrlBase 
	{
	public:
		StaticMenu(std::wstring str, EventsBase* parent);
		virtual ~StaticMenu();
		void change(std::wstring str);
		virtual void onClick(int32u id);

		//---------------------------------------------------------------------
		bool onMessage(int32u messageNo, void* data);
	protected:
		HMENU 		m_hmenu;
		WindowCtrl* m_wnd;
	};

	//-------------------------------------------------------------------------
	class PopupMenu : public CtrlBase
	{
	public:
		PopupMenu(EventsBase* parent);
		~PopupMenu() {}
		int32u show(std::wstring str, Point_i pos);

		//---------------------------------------------------------------------
		bool onMessage(int32u messageNo, void* data);
	protected:
		HMENU 		m_hmenu;
		WindowCtrl* m_wnd;
	};

	//-------------------------------------------------------------------------
	enum MenuMessages : int32u
	{
		MENU_CLICK = 2048
	};

	/** ПРЕДСТАВЛЕНИЕ МЕНЮ:
		Меню представляется в виде такой строки:
			"Файл > "        	     // > Значит, что этот пункт меню выпадащий
				"=1 #1 Новый\tCtrl+N | "          // #1 тут будет первая иконка
				"=2 #2 Открыть\tCtrl+O | "      // \tCtrl+O будет справа без \t
				"=3 #3 Сохранить\tCtrl+S | "           // | разделитель пунктов
				"--- | "                                // --- значит Separator
				"Последние > "
					"=4 ! 1.c | "                           // ! Значит Checked
					"=5 2.c | "        // =5 номер события при нажатии на пункт
					"=6 $ 3.c | "                      // $ Значит MenuBarBreak
					"=7 \t4.c | "                   // Эта надпись будет справа
					"=8 ~ 5.c "                              // ~ Значит Grayed
				"< "
				"--- | "
				"=9 #5 Выход\tAlt+F4 "
			"< "
			"Правка > "
				"=10 ... "
			"< "
			
		Что эквивалентно

			"Файл > =1 #1 Новый\tCtrl+N | =2 #2 Открыть\tCtrl+O | =3 #3 Сохранить\tCtrl+S | --- | Последние > =4 ! 1.c | =5 2.c | =6 $ 3.c | =7 \t4.c | =8 ~ 5.c < --- | =9 #5 Выход\tAlt+F4 < Правка > =10 ... <"

		Меню парсится с подобной строки. Пробелы вначале и в конце строки или управляющей конструкции не учитываются.
	 */

}

#endif // TWG_CTRL_MENU_INCLUDED