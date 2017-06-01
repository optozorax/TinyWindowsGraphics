#ifndef TWG_WINDOW_INCLUDED
#define TWG_WINDOW_INCLUDED

#include <string>

#include "twg_basics.h"
#include "twg_point.h"

namespace twg
{

	class 	WindowObject;
	class 	WindowBase;
	class 	WindowType;
	enum 	WindowStyle;

	//-------------------------------------------------------------------------
	/* Класс, который: запускает независимую обработку сообщений окна, и
	   вызывает оконную процедуру у соответствующего объекта каждый раз, когда 
	   она вызывается операционной системой.
	 */
	class WindowObject
	{
	public:
		WindowBase();

		HWND getHwnd(void);

		ImageWin 	canvas;
	protected:
		HWND 	m_hwnd;
		HDC 	m_hdc;

		virtual HWND create() = 0;
		virtual LRESULT wndProc(HWND hwnd, 
								UINT msg,
								WPARAM wParam, 
								LPARAM lParam) = 0;
	};

	//-------------------------------------------------------------------------
	/* Класс для настройки внешнего вида окна. Так же имеет интерфейс таскбара.
	 */
	class WindowBase : WindowObject
	{
	public:
		WindowStandart(WidnowType type);
		~WindowStandart();

		Point_i 	getCanvasSize(void);
		Point_i 	getWindowSize(void);
		Point_i 	getPos(void);
		Point_i 	getMaxSize(void);
		Point_i 	getMinSize(void);
		std::string getCaption(void);
		WindowStyle getStyle(void);

		void setCanvasSize(Point_i size);
		void setWindowSize(Point_i size);
		void setPos(Point_i size);
		void setMaxSize(Point_i maxSize_);
		void setMinSize(Point_i minSize_);
		void setCaption(std::string caption);
		void setStyle(WindowStyle style);

		void setTaskbarColor(TaskbarColor clr);
		void setTaskbarProgress(double progress);
		void setFullScreen(bool isFullScreen);
	protected:
		Point_i		m_minSize;
		Point_i		m_maxSize;

		HWND 	create();
		LRESULT wndProc(HWND hwnd, 
						UINT msg,
						WPARAM wParam, 
						LPARAM lParam);

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
		WindowType();

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

	enum WindowStyle 
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
	
}

#endif // TWG_WINDOW_INCLUDED