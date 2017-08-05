#ifndef TWG_WINDOW_BUFFER_INCLUDED
#define TWG_WINDOW_BUFFER_INCLUDED

#include "twg/window.h"
#include "twg/image.h"

#include "twg/image/image_drawing.h"
#include "twg/image/image_agg.h"

#include "twg/window/window_events.h"

namespace twg
{

	template<class WindowClass, class ImageClass>
	class WindowBuffer : public WindowClass
	{
	public:
		WindowBuffer(WindowType type, Point_i startSize) : 
			WindowClass(type),
			buffer(startSize) {}

		void redraw() {
			canvas.copyTo(&buffer, 
						  Point_i(0, 0), 
						  Point_i(0, 0), 
						  buffer.size());
		}

		ImageClass buffer;
	private:
		using WindowClass::canvas;
	};

	//-------------------------------------------------------------------------
	typedef WindowBuffer<WindowBase, ImageBase> 		WinBufBaseBase;
	typedef WindowBuffer<WindowBase, ImageDrawing_aa> 	WinBufBaseDraw_aa;
	typedef WindowBuffer<WindowBase, ImageDrawing_win> 	WinBufBaseDraw_win;
	typedef WindowBuffer<WindowBase, ImageAgg> 			WinBufBaseAgg;
	
	typedef WindowBuffer<WindowEvents, ImageBase> 		WinBufEventsBase;
	typedef WindowBuffer<WindowEvents, ImageDrawing_aa> WinBufEventsDraw_aa;
	typedef WindowBuffer<WindowEvents, ImageDrawing_win>WinBufEventsDraw_win;
	typedef WindowBuffer<WindowEvents, ImageAgg> 		WinBufEventsAgg;
	
}

#endif // TWG_WINDOW_BUFER_INCLUDED