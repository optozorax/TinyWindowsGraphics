#ifndef TWG_IMAGE_AGG_INCLUDED
#define TWG_IMAGE_AGG_INCLUDED

#include "twg/basics.h"
#include "twg/point.h"
#include "twg/color.h"
#include "twg/image.h"

#include <agg_scanline_u.h>
#include <agg_rasterizer_scanline_aa.h>
#include <agg_pixfmt_rgba.h>
#include <agg_rendering_buffer.h>
#include <agg_conv_stroke.h>
#include <agg_renderer_scanline.h>
#include <agg_ellipse.h>
#include <agg_basics.h>
#include <agg_renderer_primitives.h>
#include "agg_font_win32_tt.h"

namespace twg
{

	typedef agg::rendering_buffer 							rendering_buffer;
	typedef agg::pixfmt_bgra32								pixfmt;
	typedef agg::renderer_base<pixfmt>						renderer_base;
	typedef agg::renderer_scanline_aa_solid<renderer_base>	renderer_scanline;
	typedef agg::rasterizer_scanline_aa<>					rasterizer_scanline;
	typedef agg::scanline_u8								scanline;
	typedef agg::font_engine_win32_tt_int32 				font_engine_type;
	typedef agg::font_cache_manager<font_engine_type> 		font_manager_type;
	typedef agg::renderer_primitives<renderer_base> 		renderer_prim;

	//-------------------------------------------------------------------------
	class ImageAgg : virtual public ImageBase
	{
	public:
		ImageAgg(Point_i size);
		virtual ~ImageAgg() {}

		void resize(Point_i newSize);

		rendering_buffer 		rbuf;
		pixfmt 					pf;
		renderer_base 			renBase;
		renderer_scanline 		renSl;
		rasterizer_scanline 	ras;
		scanline 				sl;
		renderer_prim 			prim;

		font_engine_type		fEng;
		font_manager_type		fMan;
	protected:
		void assignBuffers();
	};

	//-------------------------------------------------------------------------
	class VertexSrc 
	{
	public:
		VertexSrc(int num = 100);
		~VertexSrc();

		void reset(void);
		void addPoint(Point_d x);
		void setIsPolygon(bool x);

		void 	rewind(int32u no);
		int32u 	vertex(double* x, double* y);

	private:
		int32u	m_num;
		int32u	m_count;
		double*	m_x;
		double*	m_y;
		bool	m_poly;
	};
	
}

#endif // TWG_IMAGE_AGG_INCLUDED