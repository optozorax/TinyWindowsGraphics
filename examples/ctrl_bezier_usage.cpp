#include "twg_window.h"
#include "twg_ctrl.h"
#include "image/twg_image_drawing.h"

class PointCtrl : Ctrl
{
public:
	PointCtrl() {}

	void setState(PointState state) 
	{
		m_current = state;
	}

	void move(Point_i diff) 
	{
		m_pos += diff;
	}

	bool onCtrl(Point_i pos)
	{
		return (m_pos-pos).getLength() < m_radius;
	}

	void onMouse(Point_i pos, MouseType type) 
	{
		bool on = onCtrl(pos);

		switch (type) {
			case MOUSE_R_UP:
				/** Вызывать меню, где можно:
						Удалить кружочек, команда: 
							текущий айди << ln(max id)/ln(2) + 1.
						Посмотреть информацию:
							Координаты
							Радиус 
				 */
				break;
			case MOUSE_WHEEL_UP:
				if (m_radius < 20)
					m_radius++;
				break;
			case MOUSE_WHEEL_DOWN:
				if (m_radius > 2)
					m_radius--;
				break;
		}

		if (m_current == POINT_MOVE && type == MOUSE_MOVE)
			move(pos-m_pos)

		switch (type) {
			case MOUSE_MOVE:
				if (on && !(m_current == POINT_MOVE))
					m_current = POINT_HOVER;
				break;
			case MOUSE_L_DOWN:
				if (on)
					m_current = POINT_MOVE;
				break;
			case MOUSE_L_UP:
				if (m_current == POINT_MOVE)
					m_current = POINT_DEFAULT;
				break;
		}
		
		switch (m_current) {
			case POINT_HOVER:
				setCursor(CURSOR_CLICK);
				break;
			case POINT_MOVE:
				setCursor(CURSOR_MOVE);
				break;
		}
	};

	void onKeyboard(int32 key, bool isDown) 
	{

		if (isDown && (m_current == POINT_MOVE || m_current == POINT_HOVER))
		switch (key) {
			case VK_UP:
				move(Point_i(0, -1));
				m_current = POINT_MOVE;
				break;
			case VK_DOWN:
				move(Point_i(0, 1));
				m_current = POINT_MOVE;
				break;
			case VK_LEFT:
				move(Point_i(-1, 0));
				m_current = POINT_MOVE;
				break;
			case VK_RIGHT:
				move(Point_i(1, 0));
				m_current = POINT_MOVE;
				break;
		}
	};

	void onMessage(int32 messageNo) 
	{
		int32u deleteMessage = (id << (int8u)(
									ln(m_window->getCtrlNum)/ln(2.0)
								)) + 1;

		if (messageNo == deleteMessage)
			m_window->deleteCtrl(id);д
	};

	void draw(void) {
		twg::ImageDrawing_aa img(m_bufer);

		img.setBrush(setAlpha(White, 128));
		img.drawPolygon(computeEllipse(m_pos), Point_d(m_radius, m_radius));

		switch (m_state) {
			case POINT_DEFAULT:
				img.setPen(Red);
				break;
			case POINT_MOVE:
				img.setPen(setAlpha(Orange,128));
				break;
			case POINT_HOVER:
				img.setPen(Orange);
				break;
		}
		img.drawPolyline(computeEllipse(m_pos), Point_d(m_radius, m_radius));
	};
private:
	enum PointState
	{
		POINT_DEFAULT,
		POINT_MOVE,
		POINT_HOVER
	};

	Point_d 			m_pos;
	static double 		m_radius;
	PointState			m_current;
};

class BezierCtrl : Ctrl 
{
public:
	BezierCtrl() {};
	~BezierCtrl() {};

	void onMouse(int32 x, int32 y, MouseType type, int32 wheel) 
	{
	};

	void onKeyboard(int32 key, bool isDown) 
	{

	};

	void drawOn(ImageBase *img) {};
private:
	std::vector<Point_d> 	m_points;
};

int main() {
	return 0;
}