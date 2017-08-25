#include <windows.h>
#include "twg/twg.h"
#include "twg/window/window_events.h"

using namespace twg;

enum CarcassMessages : int32u;
struct WhenResize;
struct WhenMove;
class WindowCarcass;

//-----------------------------------------------------------------------------
/**
	Тип окна, который обеспечивает оболочку над функциями любого вашего окна, которое имеет в предках WindowEvents, чтобы поместить это окно в каркас. 
 */
template<class Win>
class CarcassWindow : public Win
{
public: 
	CarcassWindow(EventsBase* parent) : Win(parent) {}
	CarcassWindow(WindowType type, EventsBase* parent) : Win(type, parent) {}

	bool onResize(Rect* rect, SizingType type);
	bool onMove(Point_i newPos);

	// virtual Rect 	getRect(void);
	// virtual Point_i getMaxSize(void);
	// virtual Point_i getMinSize(void);

	// virtual void 	setRect(Rect rect);
};

//typedef CarcassWindow<WindowEvents> HalfWindow;
typedef WindowEvents HalfWindow;

//-----------------------------------------------------------------------------
enum CarcassMessages : int32u
{
	CARCASS_RESIZE,
	CARCASS_MOVE
};

//-----------------------------------------------------------------------------
struct WhenResize {
	HalfWindow* pointer;
	Rect* rect;
	SizingType type;
};

//-----------------------------------------------------------------------------
struct WhenMove {
	HalfWindow* pointer;
	Point_i newPos;
};

//-----------------------------------------------------------------------------
/**
	Тип каркаса окон. Он имеет у себя два окна. Они представляются как единое целое окно. Может рассматривать их в виде: горизонтальный - одно окно справа, другое слева; вертикальный - одно окно вверху, другое внизу.
	При их ресайзинге учитываются их минимальные и максимальные размеры. 
	Сам является окном в смысле событий, но третьего лишнего окна не создает.
 */
class WindowCarcass : public HalfWindow
{
public:
	WindowCarcass(EventsBase* parent) : 
		HalfWindow(parent) {}

	void assign(HalfWindow* _wnd1, HalfWindow* _wnd2, bool isVertical);
	void* sendMessageUp(int32u messageNo, void* data);

	// void setRect(Rect rect);
	// Rect getRect(void);

	void waitForClose() {
		wnd1->waitForClose();
		wnd2->waitForClose();
	};

private:
	HalfWindow* wnd1;
	HalfWindow* wnd2;

	bool isVertical;

	bool 		notResize;
	Point_i 	max;
	Point_i 	min;
	Rect 		rect;
	double 		alpha;

	bool 		anotResize;
	Point_i 	amax;
	Point_i 	amin;
	Rect 		arect;
	double 		aalpha;
};

//=============================================================================
//=============================================================================
//=============================================================================

//-----------------------------------------------------------------------------
template<class Win>
bool CarcassWindow<Win>::onResize(Rect* rect, SizingType type) {
	bool returned = Win::onResize(rect, type);
	WhenResize when = {this, rect, type};
	sendMessageUp(CARCASS_RESIZE, &when);
	//return returned;
	return true;
}

//-----------------------------------------------------------------------------
template<class Win>
bool CarcassWindow<Win>::onMove(Point_i newPos) {
	bool returned = Win::onMove(newPos);
	WhenMove when = {this, newPos};
	sendMessageUp(CARCASS_MOVE, &when);	
	//return returned;
	return true;
}

// //-----------------------------------------------------------------------------
// template<class Win>
// Rect CarcassWindow<Win>::getRect(void) {
// 	return Win::getRect();
// }

// //-----------------------------------------------------------------------------
// template<class Win>
// Point_i CarcassWindow<Win>::getMaxSize(void) {
// 	return Win::getMaxSize();
// }

// //-----------------------------------------------------------------------------
// template<class Win>
// Point_i CarcassWindow<Win>::getMinSize(void) {
// 	return Win::getMinSize();
// }

// //-----------------------------------------------------------------------------
// template<class Win>
// void CarcassWindow<Win>::setRect(Rect rect) {
// 	Win::setRect(rect);
// }

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

/**
	Это набор функций и типов для преобразований координат. Как видно, можно преобразовывать не только типы Rect или Point_i, но и тип SizingType. Сначала его переводят в представление Point_i, а затем уже меняют координаты.
	Все эти функции нужны для обеспечения инвариантности функций, которые производят вычисления над ресайзнутым окном. Чтобы по триста раз не писать для каждого случая почти одно и то же.
	Данные функции позволяют поменять как верх с низом: changeY; так и право с лево: changeX. Так же они позволяют поменять местами x и y: changeXY - для того, чтобы обеспечить такой же набор операций для двух вертикальных окон, если есть функции для горизонтальных окон.
 */

//-----------------------------------------------------------------------------
namespace Sizing2
{
	const Point_i Top = Point_i(0, 1);
	const Point_i Bottom = Point_i(0, -1);
	const Point_i Left = Point_i(-1, 0);
	const Point_i Right = Point_i(1, 0);

	const Point_i TopLeft = Top + Left;
	const Point_i TopRight = Top + Right;
	const Point_i BottomLeft = Bottom + Left;
	const Point_i BottomRight = Bottom + Right;
}

//-----------------------------------------------------------------------------
Point_i sizing2sizing2(SizingType type) {
	switch (type) {
		case SIZING_LEFT: return Sizing2::Left;
		case SIZING_RIGHT: return Sizing2::Right;
		case SIZING_BOTTOM: return Sizing2::Bottom;
		case SIZING_TOP: return Sizing2::Top;

		case SIZING_BOTTOM_LEFT: return Sizing2::BottomLeft;
		case SIZING_BOTTOM_RIGHT: return Sizing2::BottomRight;
		case SIZING_TOP_LEFT: return Sizing2::TopLeft;
		case SIZING_TOP_RIGHT: return Sizing2::TopRight;
	}

	return Point_i(0, 0);
}

//-----------------------------------------------------------------------------
void changeY(Point_i& p) {
	p = Point_i(p.x, -p.y);
}

//-----------------------------------------------------------------------------
void changeY(Rect& rect) {
	rect = Rect(rect.ax, -rect.by, rect.bx, -rect.ay);
}

//-----------------------------------------------------------------------------
void changeX(Point_i& p) {
	p = Point_i(-p.x, p.y);
}

//-----------------------------------------------------------------------------
void changeX(Rect& rect) {
	rect = Rect(-rect.bx, rect.ay, -rect.ax, rect.by);
}

//-----------------------------------------------------------------------------
void changeXY(Rect& rect) {
	rect = Rect(rect.ay, rect.ax, rect.by, rect.bx);
}

//-----------------------------------------------------------------------------
void changeXY(Point_i& p) {
	p = Point_i(p.y, p.x);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

/**
	Здесь представлены функции, которые вычисляют новые размеры двух присоединенных окон с учетом их минимальных и максимальных размеров, с учетом прошлых положений. Функции написаны только для случая, когда окна горизонтальны, и только для левого окна. 
	Чтобы все работало и для правого окна можно преобразовать координаты функциями выше. Что и делается в функции resize.
	Чтобы все работало для вертикального окна, нужно преобразовать координаты по другим функциям выше. 
	Делать это можно потому что для правого окна все точно так же как и для левого окна, только зеркально отражено. Для вертикальный окон все точно так же, как и для горизонтальных, только поменяны местами координаты.
	Не будь этих преобразований координат, пришлось писать код для каждого случая. А когда захочешь внести изменения в логику ресайзинга, придется вносить изменения в каждый случай.
	Как бы там ни было, код все-равно сложный. Я думаю, что легче написать свой, поигравшись с программой, чем понять этот.
 */

#define updateAlpha alpha = rect.x(); \
					alpha = alpha/(alpha + arect.x()); \
					aalpha = 1-alpha;

//-----------------------------------------------------------------------------
void assign(Rect& rect, Rect& arect,
			Point_i min, Point_i max,
			Point_i amin, Point_i amax,
			double& alpha, double& aalpha) {
	if (rect.x() < min.x) rect.bx = rect.ax + min.x;
	if (rect.y() < min.y) rect.by = rect.ay + min.y;
	if (rect.x() > max.x) rect.bx = rect.ax + max.x;
	if (rect.y() > max.y) rect.by = rect.ay + max.y;

	if (arect.x() < amin.x) arect.bx = arect.ax + amin.x;
	if (arect.y() < amin.y) arect.by = arect.ay + amin.y;
	if (arect.x() > amax.x) arect.bx = arect.ax + amax.x;
	if (arect.y() > amax.y) arect.by = arect.ay + amax.y;

	arect.ay = rect.ay;
	arect.by = rect.by;

	if (arect.y() < amin.y) arect.by = arect.ay + amin.y;
	if (arect.y() > amax.y) arect.by = arect.ay + amax.y;

	rect.by = arect.by;

	arect.bx = arect.x() + rect.bx;
	arect.ax = rect.bx;

	updateAlpha;
}

//-----------------------------------------------------------------------------
void rLeftSide(Rect& rect, Rect& arect,
			   Point_i min, Point_i max,
			   Point_i amin, Point_i amax,
			   double& alpha, double& aalpha) {
	// Изменяется левая сторона левого окна
	rect.bx = rect.ax + alpha*fabs(arect.bx - rect.ax);

	// Проверяем размеры первого окна
	if (rect.x() < min.x) {
		rect.bx = rect.ax + min.x;
		updateAlpha;
	}
	if (rect.x() > max.x) {
		rect.bx = rect.ax + max.x;
		updateAlpha;
	}

	// При естественном ресайзинге
	// Это костыль, потому что когда у левого окна установлены мин и макс размеры, не передается событие о ресайзинге, выходящем за рамки этих размеров. А нам это надо, чтобы этот избыток или недостаток переправить на второе окно. Когда же левое окно ресайзят насильно, через потомка сверху, то этот костыль не нужен, и он не сработает.
	if (rect.x() == min.x && arect.x() != amin.x) {
		rect.ax++;
		rect.bx++;
		updateAlpha;
	}
	if (rect.x() == max.x && arect.x() != amax.x) {
		rect.ax--;
		rect.bx--;
		updateAlpha;
	}

	arect.ax = rect.bx;

	// Смотрим, чтобы мы не нарушили размеры второго окна
	if (arect.x() < amin.x) {
		arect.ax = arect.bx - amin.x;
		rect.bx = arect.ax;
		updateAlpha;
	}
	if (arect.x() > amax.x) {
		arect.ax = arect.bx - amax.x;
		rect.bx = arect.ax;
		updateAlpha;
	}
}

//-----------------------------------------------------------------------------
void rUpSide(Rect& rect, Rect& arect,
			 Point_i min, Point_i max,
			 Point_i amin, Point_i amax) {
	// Изменяется верхняя сторона
	// Проверяем размеры первого окна
	if (rect.y() < min.y)
		rect.ay = rect.by - min.y;
	if (rect.y() > max.y)
		rect.ay = rect.by - max.y;

	arect.ay = rect.ay;

	// Проверяем размеры второго окна
	if (arect.y() < amin.y)
		arect.ay = arect.by - amin.y;
	if (arect.y() > amax.y)
		arect.ay = arect.by - amax.y;

	rect.ay = arect.ay;
}

//-----------------------------------------------------------------------------
void rRightSide(Rect& rect, Rect& arect,
				Point_i min, Point_i max,
				Point_i amin, Point_i amax,
				double& alpha, double& aalpha) {
	arect.ax = rect.bx;

	// Изменяется правая сторона левого окна
	if (arect.x() < amin.x) {
		arect.ax = arect.bx - amin.x;
		rect.bx = arect.ax;
	}
	if (arect.x() > amax.x) {
		arect.ax = arect.bx - amax.x;
		rect.bx = arect.ax;
	}
	
	updateAlpha;
}

//-----------------------------------------------------------------------------
void resize(Point_i type, 
			Rect& rect, Rect& arect,
			Point_i min, Point_i max,
			Point_i amin, Point_i amax,
			double& alpha, double& aalpha,
			bool isLeft) {

	// Два окна всегда должны быть одной высоты и на одной высоте
	arect.ay = rect.ay;
	arect.by = arect.ay + rect.y();

	// Если это правое окно, преобразуем координаты так, чтобы оно стало левым
	if (!isLeft) {
		changeX(type);
		changeX(rect);
		changeX(arect);
	}

	// Рассматриваем два типа ресайзинга левого окна
	if (type.x < 0)
		rLeftSide(rect, arect, min, max, amin, amax, alpha, aalpha);
	else if (type.x > 0)
		rRightSide(rect, arect, min, max, amin, amax, alpha, aalpha);
	
	// Если это правое окно, возвращаем координаты так, чтобы оно снова было правым
	if (!isLeft) {
		changeX(type);
		changeX(rect);
		changeX(arect);
	}

	if (type.y > 0)
		rUpSide(rect, arect, min, max, amin, amax);
	else if (type.y < 0) {
		// Изменяем координаты так, чтобы низ стал верхом
		changeY(rect);
		changeY(arect);
		rUpSide(rect, arect, min, max, amin, amax);
		changeY(rect);
		changeY(arect);
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void WindowCarcass::assign(HalfWindow* _wnd1, HalfWindow* _wnd2, bool _isVertical) {
	wnd1 = _wnd1;
	wnd2 = _wnd2;

	notResize = true;
	anotResize = true;
	isVertical = _isVertical;

	min = wnd1->getMinSize();
	max = wnd1->getMaxSize();
	amin = wnd2->getMinSize();
	amax = wnd2->getMaxSize();

	rect = wnd1->getRect();
	arect = wnd2->getRect();

	alpha = 0;
	aalpha = 0;

	if (isVertical) {
		changeXY(rect);
		changeXY(arect);
		changeXY(min);
		changeXY(max);
		changeXY(amin);
		changeXY(amax);
	}

	::assign(rect, arect, min, max, amin, amax, alpha, aalpha);

	if (isVertical) {
		changeXY(rect);
		changeXY(arect);
		changeXY(min);
		changeXY(max);
		changeXY(amin);
		changeXY(amax);
	}

	wnd1->setRect(rect);
	wnd2->setRect(arect);
}

//-----------------------------------------------------------------------------
void  resizeWnd(bool& notResize, bool& anotResize,
				bool isLeft, bool isVertical,
				Point_i type,
				Rect& rect, Rect& arect,
				Point_i min, Point_i max,
				Point_i amin, Point_i amax,
				double& alpha, double& aalpha,
				Rect& origRect,
				HalfWindow* awnd) {
	if (!notResize) {
		anotResize = true;

		if (isVertical) {
			changeXY(type);
			changeX(type);
			changeXY(rect);
			changeXY(arect);
			changeXY(min);
			changeXY(max);
			changeXY(amin);
			changeXY(amax);
		}

		::resize(type, 
				 rect, arect, 
				 min, max, 
				 amin, amax, 
				 alpha, aalpha, 
				 isLeft);

		if (isVertical) {
			changeXY(type);
			changeXY(rect);
			changeXY(arect);
			changeXY(min);
			changeXY(max);
			changeXY(amin);
			changeXY(amax);
		}

		origRect = rect;
		awnd->setRect(arect);
	}
	notResize = false;
}

//-----------------------------------------------------------------------------
void* WindowCarcass::sendMessageUp(int32u messageNo, void* data) {
	if (messageNo == CARCASS_RESIZE) {
		WhenResize* when = data;
		if (when->pointer == wnd1) {
			resizeWnd(notResize, anotResize,
					  true, isVertical,
					  sizing2sizing2(when->type),
					  *(when->rect), arect,
					  min, max, amin, amax,
					  alpha, aalpha,
					  rect, wnd2);
		} else {
			resizeWnd(anotResize, notResize,
					  false, isVertical,
					  sizing2sizing2(when->type),
					  *(when->rect), rect,
					  amin, amax, min, max,
					  aalpha, alpha,
					  arect, wnd1);
		}
	} else 
	if (messageNo == CARCASS_MOVE) {
		WhenMove* when = data;
		if (when->pointer == wnd1) {
			anotResize = true;
			Rect newRect(when->newPos.x, when->newPos.y, when->newPos.x + rect.x(), when->newPos.y + rect.y());
			arect.ax += newRect.ax - rect.ax;
			arect.ay += newRect.ay - rect.ay;
			arect.bx += newRect.bx - rect.bx;
			arect.by += newRect.by - rect.by;
			wnd2->setRect(arect);
			rect = newRect;
		} else {
			notResize = true;
			Rect newRect(when->newPos.x, when->newPos.y, when->newPos.x + arect.x(), when->newPos.y + arect.y());
			rect.ax += newRect.ax - arect.ax;
			rect.ay += newRect.ay - arect.ay;
			rect.bx += newRect.bx - arect.bx;
			rect.by += newRect.by - arect.by;
			wnd1->setRect(rect);
			arect = newRect;
		}
	}
	return nullptr;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

int main() {
	WindowCarcass wnd(nullptr);

	WindowType type1(-1,
		Point_i(100, 100),
		Point_i(100, 100), 
		Point_i(50, 40),
		Point_i(200, 150),
		L"Left",
		WINDOW_TOOL);
	CarcassWindow<WindowEvents> wnd1(type1, &wnd);

	WindowType type2(-1,
		Point_i(100, 100),
		Point_i(100, 100), 
		Point_i(50, 50),
		Point_i(200, 110),
		L"Right",
		WINDOW_TOOL);
	CarcassWindow<WindowEvents> wnd2(type2, &wnd);

	wnd.assign(&wnd1, &wnd2, false);

	wnd.waitForClose();
}