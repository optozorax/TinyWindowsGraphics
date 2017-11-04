#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_u.h"
#include "agg_renderer_scanline.h"
#include "agg_trans_perspective.h"
#include "agg_span_allocator.h"
#include "agg_span_interpolator_trans.h"
#include "agg_span_subdiv_adaptor.h"
#include "agg_pixfmt_rgba.h"
#include "agg_image_accessors.h"
#include "agg_span_image_filter_rgba.h"
#include "twg/image/image_drawing.h"

namespace twg
{

//-----------------------------------------------------------------------------
ImageDrawing_aa::ImageDrawing_aa(ImageBase* img) : ImageBase(Point_i(10, 10)), ImageDrawing(Point_i(10, 10)), ImageAgg(Point_i(10, 10)) {
	assign(img);
	assignBuffers();
}

//-----------------------------------------------------------------------------
ImageDrawing_aa::ImageDrawing_aa(Point_i size) : ImageBase(size), ImageDrawing(size), ImageAgg(size) {

}

//-----------------------------------------------------------------------------
ImageDrawing_aa::~ImageDrawing_aa() {

}

//-----------------------------------------------------------------------------
void ImageDrawing_aa::setPen(Pen pen) {
	m_pen = pen;
}

//-----------------------------------------------------------------------------
void ImageDrawing_aa::setBrush(Brush brush) {
	m_brush = brush;
}

//-----------------------------------------------------------------------------
void ImageDrawing_aa::setTextStyle(TextStyle style) {
	m_text = style;

	if (m_text.flags & TEXT_BOLD)
		fEng.weight(100);
	else
		fEng.weight(0);
	fEng.italic(m_text.flags & TEXT_ITALIC);
	fEng.strike(m_text.flags & TEXT_ITALIC);
	fEng.underline(m_text.flags & TEXT_UNDERLINE);
	fEng.height(m_text.width);
	fEng.flip_y(true);

	std::string str(m_text.name.begin(), m_text.name.end());
	fEng.create_font(str.c_str(), agg::glyph_ren_outline);
}

//-----------------------------------------------------------------------------
void ImageDrawing_aa::drawPTo(ImageBase* dst, Polygon_d rect) {
	typedef agg::pixfmt_bgra32_pre         pixfmt_pre;
	typedef agg::renderer_base<pixfmt_pre> renderer_base_pre;
	typedef agg::image_accessor_clone<pixfmt> img_accessor_type;
	typedef agg::span_interpolator_trans<agg::trans_perspective> interpolator_type;
	typedef agg::span_image_filter_rgba_2x2<img_accessor_type, interpolator_type> span_gen_type;

	pixfmt_pre        pixf_pre(rbuf);
	renderer_base_pre rb_pre(pixf_pre);

	ras.clip_box(0, 0, width(), height());
	ras.reset();
	ras.move_to_d(rect.array[0].x, rect.array[0].y);
	ras.line_to_d(rect.array[1].x, rect.array[1].y);
	ras.line_to_d(rect.array[2].x, rect.array[2].y);
	ras.line_to_d(rect.array[3].x, rect.array[3].y);

	agg::span_allocator<pixfmt::color_type> sa;
	agg::image_filter_bilinear filter_kernel;
	agg::image_filter_lut filter(filter_kernel, false);

	rendering_buffer rbuf_img((int8u*) dst->buf(), dst->width(), dst->height(), dst->width()*4);
	pixfmt pixf_img(rbuf_img);
	img_accessor_type ia(pixf_img);
	double polygon[8];
	polygon[0] = rect.array[0].x;
	polygon[1] = rect.array[0].y;
	polygon[2] = rect.array[1].x;
	polygon[3] = rect.array[1].y;
	polygon[4] = rect.array[2].x;
	polygon[5] = rect.array[2].y;
	polygon[6] = rect.array[3].x;
	polygon[7] = rect.array[3].y;
	agg::trans_perspective tr(polygon, 0, 0, dst->width(), dst->height());
	if(tr.is_valid()) {
		interpolator_type interpolator(tr);
		span_gen_type sg(ia, interpolator, filter);

		agg::render_scanlines_aa(ras, sl, rb_pre, sa, sg);
	}
}

//-----------------------------------------------------------------------------
Pen ImageDrawing_aa::getPen(void) {
	return m_pen;
}

//-----------------------------------------------------------------------------
Brush ImageDrawing_aa::getBrush(void) {
	return m_brush;
}

//-----------------------------------------------------------------------------
TextStyle ImageDrawing_aa::getTextStyle(void) {
	return m_text;
}

//-----------------------------------------------------------------------------
Point_d ImageDrawing_aa::getTextSize(std::wstring text) {
	text += L"\n";
	while (text.find(L"\t") != std::string::npos)
		text.replace(text.find(L"\t"), 1, L"        ");
	
	SIZE *sz = new SIZE;
	std::wstring first;
	Point_i x(0, 0);

	do {
		first = text.substr(0, text.find(L'\n'));
		GetTextExtentPoint32W(fEng.getHdc(), first.c_str(), first.size(), sz);
		x = Point_i(TWG_max(x.x, sz->cx), x.y + sz->cy);
		text.erase(0, first.size()+1);
	} while (text.size() != 0);

	delete sz;
	return Point_d(x);
}

//-----------------------------------------------------------------------------
void ImageDrawing_aa::drawPolygon(Polygon_d& points) {
	VertexSrc poly(points.array.size());
	for (int i = 0; i < points.array.size(); i++) {
		poly.addPoint(points.array[i]);
	}

	ColorUnion clr(m_brush.clr);
	renSl.color(agg::rgba8(clr.rgba.r, clr.rgba.g, clr.rgba.b, clr.rgba.a));

	ras.reset();
	ras.add_path(poly);
	agg::render_scanlines(ras, sl, renSl);
}

//-----------------------------------------------------------------------------
void ImageDrawing_aa::drawPolyline(Polygon_d& points, bool isRoundJoin) {
	VertexSrc poly(points.array.size());
	for (int i = 0; i < points.array.size(); i++) {
		poly.addPoint(points.array[i]);
	}

	agg::conv_stroke<VertexSrc> stroke(poly);
	stroke.line_cap(agg::round_cap);
	stroke.line_join(agg::round_join);
	stroke.width(m_pen.thick);

	ColorUnion clr(m_pen.clr);
	renSl.color(agg::rgba8(clr.rgba.r, clr.rgba.g, clr.rgba.b, clr.rgba.a));

	ras.reset();
	ras.add_path(stroke);
	agg::render_scanlines(ras, sl, renSl);
}

//-----------------------------------------------------------------------------
void ImageDrawing_aa::drawLine(Point_d a, Point_d b) {
	VertexSrc poly(2);
	poly.addPoint(a);
	poly.addPoint(b);

	agg::conv_stroke<VertexSrc> stroke(poly);
	stroke.line_cap(agg::round_cap);
	stroke.line_join(agg::round_join);
	stroke.width(m_pen.thick);

	ColorUnion clr(m_pen.clr);
	renSl.color(agg::rgba8(clr.rgba.r, clr.rgba.g, clr.rgba.b, clr.rgba.a));

	ras.reset();
	ras.add_path(stroke);
	agg::render_scanlines(ras, sl, renSl);
}

//-----------------------------------------------------------------------------
void ImageDrawing_aa::drawText(Point_d pos, std::wstring text) {
	while (text.find(L"\t") != std::string::npos)
		text.replace(text.find(L"\t"), 1, L"        ");

	ColorUnion clr(m_pen.clr);
	renSl.color(agg::rgba8(clr.rgba.r, clr.rgba.g, clr.rgba.b, clr.rgba.a));

	double x = pos.x;
	double y = pos.y + getTextSize(L"\n").y;
	const wchar_t* p = text.c_str();

	const agg::glyph_cache* glyph;
	while(*p) {
		const agg::glyph_cache* glyph = fMan.glyph(*p);
		if(glyph) {
			fMan.add_kerning(&x, &y);
			fMan.init_embedded_adaptors(glyph, x, y);

			if(glyph->data_type == agg::glyph_data_outline) {
				if (*p == '\n') { 
					y += getTextSize(L"|").y;
					x = pos.x;
					p++;
					continue;
				}
				ras.reset();
				ras.add_path(fMan.path_adaptor());
				agg::render_scanlines(ras, sl, renSl);
			}

			x += glyph->advance_x;
			y += glyph->advance_y;
		}
		++p;
	}
}

}