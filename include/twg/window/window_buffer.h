#ifndef TWG_WINDOW_BUFER_INCLUDED
#define TWG_WINDOW_BUFER_INCLUDED

#include "twg/window.h"
#include "twg/image.h"

#include "twg/image/image_drawing.h"
#include "twg/image/image_agg.h"

#include "twg/window/window_events.h"

namespace twg
{

	template<class WindowClass, class ImageClass>
	class WindowBuffer : WindowClass
	{
	public:
		void redraw();
		ImageClass buffer;
	private:
		using WindowClass::canvas;
	};

	typedef WindowBufer<WindowBase, ImageBase> 			WindowBaseBase;
	typedef WindowBufer<WindowBase, ImageDrawing_aa> 	WindowBaseDraw_aa;
	typedef WindowBufer<WindowBase, ImageDrawing_win> 	WindowBaseDraw_win;
	typedef WindowBufer<WindowBase, ImageAgg> 			WindowBaseAgg;
	
	typedef WindowBufer<WindowEvents, ImageBase> 		WindowEventsBase;
	typedef WindowBufer<WindowEvents, ImageDrawing_aa> 	WindowEventsDraw_aa;
	typedef WindowBufer<WindowEvents, ImageDrawing_win> WindowEventsDraw_win;
	typedef WindowBufer<WindowEvents, ImageAgg> 		WindowEventsAgg;
	
}

#endif // TWG_WINDOW_BUFER_INCLUDED