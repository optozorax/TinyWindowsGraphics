#ifndef TWG_WINDOW_INCLUDED
#define TWG_WINDOW_INCLUDED

#include <string>

#include "twg/basics.h"
#include "twg/point.h"
#include "twg/image.h"

namespace twg
{

	class 	WindowObject;
	class 	WindowBase;
	class 	WindowType;
	enum 	WindowStyle : int32u;
	enum 	Icons : int32u;

	/** @defgroup Окна
		@{
	 */

	//-------------------------------------------------------------------------
	/** Класс, который: запускает независимую обработку сообщений окна, и
	   вызывает оконную процедуру у соответствующего объекта каждый раз, когда 
	   она вызывается операционной системой.
	 */
	class WindowObject
	{
	public:
		~WindowObject();

		/** @return HWND данного окна. */
		HWND getHwnd(void);
		/** Ожидает закрытия окна. */
		void waitForClose(void);
		/** @return true, если окно закрыто, иначе false. */
		bool isClosed(void);

		/** ImageWin данного окна. Содержит в себе HDC. */
		ImageWin 	canvas;
	protected:
		HWND 	m_hwnd;
		HDC 	m_hdc;

		/** Потомок обязан вызвать эту функцию в своем конструкторе. */
		void onStart();
		/** В эту процедуру передается указатель на wndProc, потомок здесь обязан создать окно и вернуть его HWND. */
		virtual HWND create(void* wndProc) = 0;
		/** Здесь обрабатываются события данного окна. */
		virtual LRESULT wndProc(HWND hwnd, 
								UINT msg,
								WPARAM wParam, 
								LPARAM lParam) = 0;

		friend DWORD WINAPI 	makeWindow(LPVOID data);
		friend LRESULT CALLBACK wndProc1(HWND hwnd, 
										UINT msg, 
										WPARAM wParam, 
										LPARAM lParam);
	};

	//-------------------------------------------------------------------------
	/* Класс для настройки внешнего вида окна. */
	class WindowBase : public WindowObject
	{
	public:
		/** Создает окно, все его параметры задаются в WindowType. Окно существует пока существует объект. */
		WindowBase(WindowType type);
		/** При вызове деструктора, окно уничтожается. */
		~WindowBase();

		/** @return Rect текущего окна. См. \ref Rect. */
		Rect 		 getRect(void);
		/** @return размер части окна, где доступно рисование. */
		Point_i 	 getClientSize(void);
		/** @return полный размер всего окна. */
		Point_i 	 getWindowSize(void);
		/** @return положение левого верхнего угла окна относительно левого верхнего угла экрана. */
		Point_i 	 getPos(void);
		/** @return максимальный размер данного окна. */
		Point_i 	 getMaxSize(void);
		/** @return минимальный размер данного окна.
			@note Если раннее был установлен слишком маленький минимальный размер, то он корректируется таким образом, чтобы окно поддерживало такой минимальный размер. Возвращается именно откоректированное значение. Корректировка не происходит моментально.
		 */
		Point_i 	 getMinSize(void);
		/** @return текущий заголовок окна. */
		std::wstring getCaption(void);
		/** @return текущий стиль окна. См. \ref WindowStyle. */
		WindowStyle  getStyle(void);

		/** Устанавливает одновременно и положение и размеры данного окна. См. \ref Rect. 
			@note Во время вызова события изменения размеров окна, можно менять размеры при помощи данной функции. При этом изменение размеров и положения будет стабильно. 
		 */
		void setRect(Rect rect);
		/** Устанавливает размер окна таким образом, чтобы клиентская часть была размера size. */
		void setClientSize(Point_i size);
		/** Устанавливает размер окна. */
		void setWindowSize(Point_i size);
		/** Устанавливает позицию окна. */
		void setPos(Point_i pos);
		/** Устанавливает максимальный размер окна. */
		void setMaxSize(Point_i maxSize);
		/** Устанавливает минимальный размер окна. */
		void setMinSize(Point_i minSize);
		/** Устанавливает заголовок. */
		void setCaption(std::wstring caption);
		/** Устанавливает стиль окна. См. \ref WindowStyle. */
		void setStyle(WindowStyle style);

		/** Переводит координаты  точки, заданной на экране в координаты точки, если бы она находилась в клиентской области текущего окна. */
		Point_i global2client(Point_i globalPos);
		/** Переводит координаты точки, заданной в клиентской области текущего окна, в координаты на экране. Пример использования: создание меню выскакивающего меню - `window_ctrl_test.cpp`. */
		Point_i client2global(Point_i clientPos);

		/** Сообщает окну, что его необходимо перерисовать. После вызова этой функции окну придет сообщение WM_PAINT(низкоуровневая часть). Если у окна есть автоматическое отслеживание этого сообщения, то окно просто перерисуется через некоторое время. */
		void invalidateScreen(void);
		/** Если окно имеет стиль WINDOW_IMAGE32, то обновление картинки у окна такого типа осуществляется только через эту функцию. Для других типов окон эта функция не делает ничего.
			Передается прозрачное изображение, которое нарисуется на окне.
		 */
		bool updateImage(ImageBase& img);

		// Надо ли? Это вносит лишь лишних костылей, а пользы немного.
		// void setTaskbarColor(TaskbarColor clr);
		// void setTaskbarProgress(double progress);
		// void setFullScreen(bool isFullScreen);
	protected:
		Point_i		 m_minSize;
		Point_i		 m_maxSize;
		WindowType*	 m_type;
		std::wstring m_className;
		bool 		 m_isResized;
		Rect* 		 m_nowRect;
	
		HWND 	create(void* wndProc);
		LRESULT wndProc(HWND hwnd, 
						UINT msg,
						WPARAM wParam, 
						LPARAM lParam);

		/** При наследовании от WindowBase необходимо переопределить эту функцию для получения сообщений. Сюда посылаются все сообщения, которые не обработал WindowBase. */
		virtual LRESULT wndProcNext(HWND hwnd, 
									UINT msg,
									WPARAM wParam, 
									LPARAM lParam);
	};

	//-------------------------------------------------------------------------
	/** Структура, в которой находятся все параметры создаваемого окна. */
	class WindowType 
	{
	public:
		WindowType(int32		iconNo,
				   Point_i		pos,
				   Point_i		size,
				   Point_i		minSize,
				   Point_i		maxSize,
				   std::wstring 	caption,
				   WindowStyle	style) : 
			iconNo(iconNo),
			pos(pos),
			size(size),
			minSize(minSize),
			maxSize(maxSize),
			caption(caption),
			style(style) {}

		/** Номер иконки. Сюда можно написать тот номер, который вы указали в файле ресурсов `*.rc`, либо одно из стандартных значений, записанных в \ref Icons. Так же можно указать \ref stdIcon. */
		int32		iconNo;
		/** Начальная позиция левого верхнего угла окна. */
		Point_i		pos;
		/** Начальный размер окна. Это тот размер, который задается setWindowSize. Задать размер клиентской области окна можно только после его создания при помощи \ref setClientSize. */
		Point_i		size;
		/** Минимальные размеры окна. Можно указать стандартные размеры \ref stdMin. */
		Point_i		minSize;
		/** Максимальные размеры окна. Можно указать стандартные размеры \ref stdMax. */
		Point_i		maxSize;
		/** Заголовок окна. */
		std::wstring caption;
		/** Стиль окна. См \ref WindowStyle. */
		WindowStyle	style;
	};

	//-------------------------------------------------------------------------
	/** Стиль окна. */
	enum WindowStyle : int32u
	{
		// Среди этих можно выбрать только один
		WINDOW_STANDART 	= 0, ///< Картинка
		WINDOW_TOOL 		= 1, ///< Картинка
		WINDOW_CAPTION 		= 2, ///< Картинка
		WINDOW_POPUP 		= 3, ///< Картинка
		WINDOW_NO_BORDER 	= 4, ///< Картинка
		WINDOW_IMAGE32 		= 5, ///< Картинка

		// Чтобы получить верхнии стили надо WindowStyle % WINDOW_MAX_TYPE
		WINDOW_MAX_TYPE 	= 0x010, //< Не используется пользователями.

		//---------------------------------------------------------------------
		/// Стили далее могут объединяться через оператор |.
		WINDOW_ON_TOP 		= 0x010, ///< Поверх всех окон.
		WINDOW_DISABLED 	= 0x020, ///< Изначально не принимает события.
		WINDOW_MAXIMIZED 	= 0x040, ///< Изначально на весь экран.
		WINDOW_MAX_BUTTON 	= 0x080, ///< Имеет кнопку развертки на весь экран.
		WINDOW_MIN_BUTTON 	= 0x100, ///< Имеет кнопку сворачивания.

		// Данный стиль можно задать только при создании окна
		WINDOW_DROP_SHADOW 	= 0x200 ///< Отбрасывает тень. Выпадающее меню тоже отбрасывает тень.
	};

	// enum TaskbarColor
	// {
	// 	TASKBAR_GREEN,
	// 	TASKBAR_YELLOW,
	// 	TASKBAR_RED,
	// 	TASKBAR_LOADING,
	// 	TASKBAR_BLINK
	// };

	/** Иконки, которые можно установить на окно. */
	enum Icons : int32u
	{
		ICON_APPLICATION 	= 32512,
		ICON_INFORMATION 	= 32516,
		ICON_ERROR			= 32513,
		ICON_WARNING 		= 32515,
		ICON_QUESTION 		= 32514
	};

	/** Стандартная иконка. */
	const int32   stdIcon = ICON_APPLICATION;
	/** Стандартные минимальные размеры окна. */
	const Point_i stdMin = Point_i(0, 0);
	/** Стандартные максимальные размеры окна. */
	const Point_i stdMax = Point_i(20000, 20000);

	/**
		@}
	 */

}

#endif // TWG_WINDOW_INCLUDED