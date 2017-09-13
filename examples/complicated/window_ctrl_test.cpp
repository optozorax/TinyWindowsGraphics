#include <windows.h>
#include <cstdlib>
#include <sstream>
#include "twg/twg.h"
#include "twg/window/window_ctrl.h"
#include "twg/image/image_drawing.h"
#include "twg/ctrl/menu.h"
#include <twg/ctrl/movable_ctrl.h>

using namespace twg;

class MyMenu;
class PointCtrl;

//-----------------------------------------------------------------------------
class MyMenu : public StaticMenu
{
public:
	MyMenu(std::wstring str, EventsBase* parent) : StaticMenu(str, parent) {}
	void onClick(int32u id);
};

//-----------------------------------------------------------------------------
class PointCtrl : public MovableCtrl
{
public:
	PointCtrl(Point_d pos, EventsBase* parent) : 
		MovableCtrl(parent), 
		m_pos(pos) {
			if (!m_drawed)
				redrawBuffers();
		}

	bool onMessage(int32u messageNo, void* data);
private:
	Point_d 				m_pos;

	static int32u 			m_radius;
	static int32u 			m_minRadius;
	static int32u 			m_maxRadius;

	static bool				m_drawed;
	static bool 			m_resized;
	static ImageDrawing_aa  m_imgMove;
	static ImageDrawing_aa  m_imgHover;
	static ImageDrawing_aa  m_imgDefault;

	void redrawBuffers();

	//-------------------------------------------------------------------------
	bool isInside(Point_i pos);
	void move(Point_i diff);

	bool onRMouse(Point_i pos);
	bool onWheel(Point_i pos, MouseType wheel);

	void drawDefault(ImageBase* buffer);
	void drawHover(ImageBase* buffer);
	void drawWhenMove(ImageBase* buffer);
};

//=============================================================================
//=============================================================================
//=============================================================================

//-----------------------------------------------------------------------------
int32u 			PointCtrl::m_radius(5);
int32u			PointCtrl::m_minRadius(3);
int32u 			PointCtrl::m_maxRadius(30);

ImageDrawing_aa  PointCtrl::m_imgDefault(Point_i(10, 10));
ImageDrawing_aa  PointCtrl::m_imgHover(Point_i(10, 10));
ImageDrawing_aa  PointCtrl::m_imgMove(Point_i(10, 10));
bool 			PointCtrl::m_drawed(false);
bool 			PointCtrl::m_resized(false);

//-----------------------------------------------------------------------------
void PointCtrl::redrawBuffers() {
	Point_d r(m_radius, m_radius);
	m_imgMove.resize(Point_i(r*2 + Point_d(6, 6)));
	m_imgHover.resize(Point_i(r*2 + Point_d(6, 6)));
	m_imgDefault.resize(Point_i(r*2 + Point_d(6, 6)));

	Polygon_d poly = computeEllipse(r);
	poly.move(r + Point_d(3, 3));

	m_imgDefault.clear(Transparent);
	m_imgHover.clear(Transparent);
	m_imgMove.clear(Transparent);

	m_imgDefault.setBrush(Brush(setAlpha(White, 128)));
	m_imgDefault.setPen(Pen(1, Red));
	m_imgDefault.drawPolygon(poly);
	m_imgDefault.drawPolyline(poly);

	m_imgHover.setBrush(Brush(setAlpha(Orange, 128)));
	m_imgHover.drawPolygon(poly);

	m_imgMove.setBrush(Brush(setAlpha(Orange, 50)));
	m_imgMove.drawPolygon(poly);

	m_drawed = true;
	m_resized = true;
}

//-----------------------------------------------------------------------------
void PointCtrl::move(Point_i diff) {
	m_pos += diff;
}

//-----------------------------------------------------------------------------
bool PointCtrl::isInside(Point_i pos) {
	return (m_pos-pos).getLength() < m_radius;
}

//-----------------------------------------------------------------------------
bool PointCtrl::onRMouse(Point_i pos) {
	std::wstringstream sout;
	sout << L"=1 Delete | --- | ~ Coords: (" << m_pos.x << L", " << m_pos.y << L") | ~ Radius: " << m_radius << L" | ~ ID: " << id;
	
	PopupMenu* menu = new PopupMenu(m_parent);
	m_storage->array.push_back(menu);
	if (menu->show(sout.str(), pos))
		m_storage->deleteMe(this);
	return true;
}

//-----------------------------------------------------------------------------
bool PointCtrl::onMessage(int32u messageNo, void* data) {
	return false;
}

//-----------------------------------------------------------------------------
bool PointCtrl::onWheel(Point_i pos, MouseType wheel) {
	switch (wheel) {
		case MOUSE_WHEEL_UP:
			if (m_radius < m_maxRadius && !m_resized) {
				m_radius++;
				redrawBuffers();
				return true;
			}
			break;
		case MOUSE_WHEEL_DOWN:
			if (m_radius > m_minRadius && !m_resized) {
				m_radius--;
				redrawBuffers();
				return true;
			}
			break;
	}
	return false;
}

//-----------------------------------------------------------------------------
void PointCtrl::drawDefault(ImageBase* buffer) {
	m_resized = false;
	m_imgDefault.drawTo(buffer, m_pos - Point_i(m_radius+3, m_radius+3), Point_i(0, 0), m_imgDefault.size());	
}

//-----------------------------------------------------------------------------
void PointCtrl::drawHover(ImageBase* buffer) {
	m_resized = false;
	m_imgHover.drawTo(buffer, m_pos - Point_i(m_radius+3, m_radius+3), Point_i(0, 0), m_imgHover.size());	
}

//-----------------------------------------------------------------------------
void PointCtrl::drawWhenMove(ImageBase* buffer) {
	m_resized = false;
	m_imgMove.drawTo(buffer, m_pos - Point_i(m_radius+3, m_radius+3), Point_i(0, 0), m_imgMove.size());	
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void MyMenu::onClick(int32u id) {
	static int32u pointCount = 0;
	if (id == 1) {
		pointCount++;

		Point_i size = m_wnd->getClientSize();
		Point_i pos(std::rand()*size.x/RAND_MAX, std::rand()*size.y/RAND_MAX);
		m_storage->array.push_back(new PointCtrl(pos, m_parent));
	} else if (id == 2) {
		m_wnd->storage.OMFOC = !m_wnd->storage.OMFOC;
		std::wstringstream sout;
		sout << L"=1 New point | Settings > =2 " << ((m_wnd->storage.OMFOC) ? L"!" : L" ") << L" One message for one ctrl < =3 About";
		change(sout.str());
	} else if (id == 3) {
		std::wstring text(L"This is program that shows how WindowCtrl, CtrlStorage, CtrlBase, StaticMenu, PopupMenu classes work.\nIn window you can see menu and many red points. You can:\n\tMove these points with mouse.\n\tChange size of points with mouse wheel, when cursor is on any point.\n\tCall popup menu of each point.\n\tDelete point in popup menu.\n\tDiscover information about point in popup menu.\n\tAdd a new points(new points were added in random position).\n\tChange message handle regime of controls storage:\n\t\tOne message for one control.\n\t\tOne message for each control.");
		messageBox(text, L"About", MESSAGE_OK);
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

int main() {
	WindowType type(stdIcon,
		Point_i(100, 0),
		Point_i(300, 300), 
		stdMin,
		stdMax,
		L"There are many points.",
		WINDOW_STANDART);	
	WindowCtrl wnd(type);

	MyMenu* menu = new MyMenu(L" ", &wnd.storage);
	wnd.storage.array.push_back(menu);
	menu->onClick(2);

	// Make points
	for (int i = 0; i < 5000; ++i)
		menu->onClick(1);

	wnd.redraw();
	wnd.waitForClose();
	return 0;
}