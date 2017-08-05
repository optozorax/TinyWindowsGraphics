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

	//-------------------------------------------------------------------------
	/* Класс, который: запускает независимую обработку сообщений окна, и
	   вызывает оконную процедуру у соответствующего объекта каждый раз, когда 
	   она вызывается операционной системой.
	 */
	class WindowObject
	{
	public:
		WindowObject();

		HWND getHwnd(void);

		virtual HWND create(void*) {}
		virtual LRESULT wndProc(HWND hwnd, 
								UINT msg,
								WPARAM wParam, 
								LPARAM lParam) {}

		ImageWin 	canvas;
	protected:
		HWND 	m_hwnd;
		HDC 	m_hdc;
	};

	//-------------------------------------------------------------------------
	/* Класс для настройки внешнего вида окна. Так же имеет интерфейс таскбара.
	 */
	class WindowBase : public WindowObject
	{
	public:
		WindowBase(WindowType type);
		~WindowBase();  // Окно автоматически закрывается, здесь вызывается функция по закрытия окна.

		Point_i 	getClientSize(void);
		Point_i 	getWindowSize(void);
		Point_i 	getPos(void);
		Point_i 	getMaxSize(void);
		Point_i 	getMinSize(void);
		std::string getCaption(void);
		WindowStyle getStyle(void);

		void setClientSize(Point_i size);
		void setWindowSize(Point_i size);
		void setPos(Point_i size);
		void setMaxSize(Point_i maxSize);
		void setMinSize(Point_i minSize);
		void setCaption(std::string caption);
		void setStyle(WindowStyle style);

		void invalidateScreen(void);

		// Надо ли? Это вносит лишь лишних костылей, а пользы немного.
		// void setTaskbarColor(TaskbarColor clr);
		// void setTaskbarProgress(double progress);
		// void setFullScreen(bool isFullScreen);

		HWND 	create(void*);
		LRESULT wndProc(HWND hwnd, 
						UINT msg,
						WPARAM wParam, 
						LPARAM lParam);
	protected:
		Point_i		m_minSize;
		Point_i		m_maxSize;

		// По умолчанию возвращает DefWindowProc(hwnd, msg, wParam, lParam);
		virtual LRESULT wndProcNext(HWND hwnd, 
									UINT msg,
									WPARAM wParam, 
									LPARAM lParam);
	};

	//-------------------------------------------------------------------------
	class WindowType 
	{
	public:
		WindowType(int32		iconNo,
				   Point_i		pos,
				   Point_i		size,
				   Point_i		minSize,
				   Point_i		maxSize,
				   std::string 	caption,
				   WindowStyle	style) : 
			iconNo(iconNo),
			pos(pos),
			size(size),
			minSize(minSize),
			maxSize(maxSize),
			caption(caption),
			style(style) {}

		int32		iconNo;
		Point_i		pos;
		Point_i		size;
		Point_i		minSize;
		Point_i		maxSize;
		std::string caption;
		WindowStyle	style;
	private:
		void* 		m_thisWindow;
	};

	enum WindowStyle : int32u
	{
		// Среди этих можно выбрать только один
		WINDOW_STANDART 	= 0,
		WINDOW_TOOL 		= 1,
		WINDOW_CAPTION 		= 2,
		WINDOW_POPUP 		= 3,
		WINDOW_NO_BORDER 	= 4,

		// Стили далее могут объединяться через оператор |
		WINDOW_DROP_SHADOW 	= 0x010,
		WINDOW_ON_TOP 		= 0x020,
		WINDOW_DISABLED 	= 0x040,
		WINDOW_MAXIMIZED 	= 0x080,
		WINDOW_MAX_BUTTON 	= 0x100,
		WINDOW_MIN_BUTTON 	= 0x200
	};

	// enum TaskbarColor
	// {
	// 	TASKBAR_GREEN,
	// 	TASKBAR_YELLOW,
	// 	TASKBAR_RED,
	// 	TASKBAR_LOADING,
	// 	TASKBAR_BLINK
	// };

}

#endif // TWG_WINDOW_INCLUDED