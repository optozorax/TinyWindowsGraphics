#include "twg/image/image_agg.h"

namespace twg
{

//-----------------------------------------------------------------------------
ImageAgg::ImageAgg(Point_i size) : ImageBase(size), rbuf((int8u*) buf(), width(), height(), width()*4), pf(rbuf), renBase(pf), renSl(renBase) {

}

//-----------------------------------------------------------------------------
VertexSrc::VertexSrc(int num) : m_num(0), m_count(0), m_poly(true) { 
	m_x = new double[num+5];
	m_y = new double[num+5];
	memset(m_x, 0, sizeof(double)*(num+5));
	memset(m_y, 0, sizeof(double)*(num+5));
}

//-----------------------------------------------------------------------------
VertexSrc::~VertexSrc() {
	delete[] m_x;
	delete[] m_y;
}

//-----------------------------------------------------------------------------
void VertexSrc::reset(void) {
	m_num = 0;
}

//-----------------------------------------------------------------------------
void VertexSrc::addPoint(double x, double y) {
	m_x[m_num] = x;
	m_y[m_num] = y;
	m_num++;
}

//-----------------------------------------------------------------------------
void VertexSrc::setIsPolygon(bool x) {
	m_poly = x;
}

//-----------------------------------------------------------------------------
void VertexSrc::rewind(unsigned) {
	m_count = 0;
}

//-----------------------------------------------------------------------------
unsigned VertexSrc::vertex(double* x, double* y) {
	if(m_count == m_num) {
		m_count++;
		if (m_poly) {
			if (m_num == 2)	return agg::path_cmd_stop;
			else			return agg::path_cmd_end_poly | agg::path_flags_close;
		} else 
			return agg::path_cmd_stop;
	}
	if(m_count > m_num) return agg::path_cmd_stop;
	*x = m_x[m_count];
	*y = m_y[m_count];
	m_count++;
	return ((m_count == 1) ? agg::path_cmd_move_to : agg::path_cmd_line_to);
}

}