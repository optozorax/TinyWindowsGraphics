#include <twg/twg.h>
#include <twg/window/window_ctrl.h>
#include <twg/image/image_drawing.h>
#include <twg/ctrl/movable_ctrl.h>

using namespace twg;

//-----------------------------------------------------------------------------
class Slider : public MovableCtrl
{
public:
	Slider(Point_d a, Point_d b, double& pos, EventsBase* parent) : MovableCtrl(parent), m_a(a), m_b(b), m_pos(pos), m_realPos(pos) {}

private:
	Point_d		m_a;
	Point_d		m_b;
	double&		m_pos;
	double		m_realPos;

	bool isInside(Point_i pos);
	void move(Point_i diff);

	void drawDefault(ImageBase* buffer);
	void drawHover(ImageBase* buffer);
	void drawWhenMove(ImageBase* buffer);
};

//=============================================================================
//=============================================================================
//=============================================================================

//-----------------------------------------------------------------------------
bool Slider::isInside(Point_i pos) {
	return (pos - (m_a + (m_b - m_a)*m_pos)).getLength() <= 5;
}

//-----------------------------------------------------------------------------
void Slider::move(Point_i diff) {
	Point_d lastPos = m_a + m_pos*(m_b - m_a);
	Point_d realLastPos = m_a + m_realPos*(m_b - m_a);
	Point_d c = realLastPos + diff;
	Point_d ab = m_a - m_b;
	Point_d ac = m_a - c;
	double pos = (ac.x*ab.x + ac.y*ab.y)/(ab.x*ab.x + ab.y*ab.y);
	m_realPos = pos;
	if (pos > 1) pos = 1;
	if (pos < 0) pos = 0;
	m_pos = pos;
}

//-----------------------------------------------------------------------------
void Slider::drawDefault(ImageBase* buffer) {
	ImageDrawing_aa img(buffer);
	img.setPen(Pen(3, setAlpha(Gray, 128)));
	img.setBrush(Brush(setAlpha(Red, 128)));
	img.drawLine(m_a, m_b);
	Polygon_d ellipse = computeEllipse(Point_d(5, 5));
	ellipse.move(m_a + (m_b - m_a)*m_pos);
	img.drawPolygon(ellipse);
}

//-----------------------------------------------------------------------------
void Slider::drawHover(ImageBase* buffer) {
	ImageDrawing_aa img(buffer);
	img.setPen(Pen(3, setAlpha(Gray, 128)));
	img.setBrush(Brush(setAlpha(Orange, 128)));
	img.drawLine(m_a, m_b);
	Polygon_d ellipse = computeEllipse(Point_d(5, 5));
	ellipse.move(m_a + (m_b - m_a)*m_pos);
	img.drawPolygon(ellipse);
}

//-----------------------------------------------------------------------------
void Slider::drawWhenMove(ImageBase* buffer) {
	ImageDrawing_aa img(buffer);
	img.setPen(Pen(3, setAlpha(Gray, 128)));
	img.setBrush(Brush(setAlpha(Orange, 50)));
	img.drawLine(m_a, m_b);
	Polygon_d ellipse = computeEllipse(Point_d(5, 5));
	ellipse.move(m_a + (m_b - m_a)*m_pos);
	img.drawPolygon(ellipse);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WindowType type(stdIcon, 
		Point_i(100, 100), 
		Point_i(300, 300),
		stdMin, stdMax,
		L"MovableCrtl test.",
		WINDOW_STANDART);
	WindowCtrl wnd(type);

	double pos = 0.5, pos1 = 0.1, pos2 = 0.9;
	wnd.storage.array.push_back(new Slider(Point_d(100, 100), Point_d(200, 200), pos, &wnd.storage));

	wnd.storage.array.push_back(new Slider(Point_d(100, 50), Point_d(200, 50), pos, &wnd.storage));
	wnd.storage.array.push_back(new Slider(Point_d(100, 35), Point_d(200, 35), pos1, &wnd.storage));
	wnd.storage.array.push_back(new Slider(Point_d(100, 20), Point_d(200, 20), pos2, &wnd.storage));

	wnd.storage.array.push_back(new Slider(Point_d(50, 100), Point_d(50, 200), pos, &wnd.storage));
	wnd.storage.array.push_back(new Slider(Point_d(35, 100), Point_d(35, 200), pos1, &wnd.storage));
	wnd.storage.array.push_back(new Slider(Point_d(20, 100), Point_d(20, 200), pos2, &wnd.storage));
	wnd.redraw();

	wnd.waitForClose();
}