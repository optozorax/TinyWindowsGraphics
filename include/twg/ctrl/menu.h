#ifndef TWG_CTRL_MENU_INCLUDED
#define TWG_CTRL_MENU_INCLUDED

#include <string>
#include "twg/events.h"
#include "twg/ctrl.h"

namespace twg
{

	//-------------------------------------------------------------------------
	enum MenuMessages : int32u
	{
		MENU_CLICK = int32u(2048),
		MENU_CHANGE = int32u(2049)
	};

	class Menu : public CtrlBase
	{
	public:
		Menu(std::wstring str, bool isPopup, Point_i pos, EventsBase* parent);

		void makeMenu(std::wstring str, bool isPopup);

		~Menu();
		/**
			Меню представляется в виде такой строки:
				L"Файл > "            // > Значит, что этот пункт меню выпадащий
					L"=1 #1 Новый\tCtrl+N | "      // #1 тут будет первая иконка
					L"=2 #2 Открыть\tCtrl+O | "  // \tCtrl+O будет справа без \t
					L"=3 #3 Сохранить\tCtrl+S | "       // | разделитель пунктов
					L"--- | "                            // --- значит Separator
					L"Последние > "
						L"=4 ! 1.c | "                       // ! Значит Checked
						L"=5 2.c | "    // =5 номер события при нажатии на пункт
						L"=6 $ 3.c | "                  // $ Значит MenuBarBreak
						L"=7 \t4.c | "               // Эта надпись будет справа
						L"=8 ~ 5.c "                          // ~ Значит Grayed
					L"< "
					L"--- | "
					L"=9 #5 Выход\tAlt+F4 "
				L"< "
				L"Правка > "
					L"=10 ... "
				L"< "
				
			Что эквивалентно

				L"Файл > =1 #1 Новый\tCtrl+N | =2 #2 Открыть\tCtrl+O | =3 #3 Сохранить\tCtrl+S | --- | Последние > =4 ! 1.c | =5 2.c | =6 $ 3.c | =7 \t4.c | =8 ~ 5.c < --- | =9 #5 Выход\tAlt+F4 < Правка > =10 ... <"

			Меню парсится с подобной строки. Пробелы вначале и в конце строки или управляющей конструкции не учитываются.
			Если !isPopup то меню создается так, чтобы принадлежать окну, иначе создается полностью как POPUP меню, но hwnd все-равно надо знать, чтобы обработать сообщения потом.
		*/

		// Здесь будет обрабатываться сообщение от винды WM_COMMAND, и при помощи sendMessageUp будет отправляться сообщение о нажатии пункта меню под каким-то номером события. Так же здесь должна быть обработка сообщения WM_UNINITMENUPOPUP когда меню удаляется, чтобы автоматически удалить себя, только если меню POPUP.
		bool onMessage(int32u messageNo, void* data);

		// Может быть переопределено для особого поведения.
		virtual void onClick(int32u id) { sendMessageUp(MENU_CLICK, id); };
	private:
		HMENU	m_hmenu;
		HWND 	m_hwnd;
	};

}

#endif // TWG_CTRL_MENU_INCLUDED