#include <windows.h>
#include "twg/twg.h"
#include "twg/window/window_events.h"

using namespace twg;

class CarcassedWindow;
enum CarcassMessages : int32u;
struct WhenResize;
struct WhenMove;
class WindowCarcass;

//-----------------------------------------------------------------------------
class CarcassedWindow
{
public:
	virtual void c_waitForClose(bool isAllClosed) = 0;
	virtual bool c_isClosed(void) = 0;

	virtual Rect 	c_getRect(void) = 0;
	virtual Point_i c_getMaxSize(void) = 0;
	virtual Point_i c_getMinSize(void) = 0;

	virtual void c_setRect(Rect rect) = 0;

	virtual void* c_sendMessageUp(int32u messageNo, void* data) {
		if (parent != nullptr)
			return parent->c_sendMessageUp(messageNo, data);
		return 
			nullptr;
	};

	bool 			 isFirst;
	CarcassedWindow* parent;
};

//-----------------------------------------------------------------------------
/**
	Тип окна, который обеспечивает оболочку над функциями любого вашего окна, которое имеет в предках WindowEvents, чтобы поместить это окно в каркас. 
 */
template<class Win>
class CarcassTemplate : public Win, public CarcassedWindow
{
public: 
	CarcassTemplate(WindowType type, EventsBase* parent = nullptr) : 
		Win(type, parent) {}

	bool onResize(Rect rect, SizingType type);
	bool onMove(Point_i newPos);

	//-------------------------------------------------------------------------
	void c_waitForClose(bool isAllClosed);
	bool c_isClosed(void);

	Rect 	c_getRect(void);
	Point_i c_getMaxSize(void);
	Point_i c_getMinSize(void);

	void c_setRect(Rect rect);
};

//-----------------------------------------------------------------------------
enum CarcassMessages : int32u
{
	CARCASS_RESIZE,
	CARCASS_MOVE
};

//-----------------------------------------------------------------------------
struct WhenResize {
	bool isFirst;
	Rect rect;
	SizingType type;
};

//-----------------------------------------------------------------------------
struct WhenMove {
	bool isFirst;
	Point_i newPos;
};

//-----------------------------------------------------------------------------
/**
	Тип каркаса окон. Он имеет у себя два окна. Они представляются как единое целое окно. Может рассматривать их в виде: горизонтальный - одно окно справа, другое слева; вертикальный - одно окно вверху, другое внизу.
	При их ресайзинге учитываются их минимальные и максимальные размеры. 
	Сам является окном в смысле событий.
 */
class WindowCarcass : public CarcassedWindow
{
public:
	void assign(CarcassedWindow* _wnd1, 
			    CarcassedWindow* _wnd2, 
			    bool isVertical);

	//-------------------------------------------------------------------------
	void c_waitForClose(bool isAllClosed);
	bool c_isClosed(void);

	Rect 	c_getRect(void);
	Point_i c_getMaxSize(void);
	Point_i c_getMinSize(void);

	void c_setRect(Rect rect);

	void* c_sendMessageUp(int32u messageNo, void* data);
private:
	CarcassedWindow* wnd1;
	CarcassedWindow* wnd2;

	bool 		isVertical;
	bool		isSetRect;

	// Параметры первого окна
	bool 		notResize;
	Point_i 	max;
	Point_i 	min;
	Rect 		rect;
	double 		alpha;

	// Параметры второго окна
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
bool CarcassTemplate<Win>::onResize(Rect rect, SizingType type) {
	bool returned = Win::onResize(rect, type);
	WhenResize when = {isFirst, rect, type};
	c_sendMessageUp(CARCASS_RESIZE, &when);
	return returned;
}

//-----------------------------------------------------------------------------
template<class Win>
bool CarcassTemplate<Win>::onMove(Point_i newPos) {
	bool returned = Win::onMove(newPos);
	WhenMove when = {isFirst, newPos};
	c_sendMessageUp(CARCASS_MOVE, &when);	
	return returned;
}

//-----------------------------------------------------------------------------
template<class Win>
void CarcassTemplate<Win>::c_setRect(Rect rect) {
	setRect(rect);
}

//-----------------------------------------------------------------------------
template<class Win>
void CarcassTemplate<Win>::c_waitForClose(bool isAllClosed) {
	if (isAllClosed) {
		waitForClose();
	}
}

//-----------------------------------------------------------------------------
template<class Win>
bool CarcassTemplate<Win>::c_isClosed(void) {
	return isClosed();
}

//-----------------------------------------------------------------------------
template<class Win>
Rect CarcassTemplate<Win>::c_getRect(void) {
	return getRect();
}

//-----------------------------------------------------------------------------
template<class Win>
Point_i CarcassTemplate<Win>::c_getMaxSize(void) {
	return getMaxSize();
}

//-----------------------------------------------------------------------------
template<class Win>
Point_i CarcassTemplate<Win>::c_getMinSize(void) {
	return getMinSize();
}

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
SizingType sizing22sizing(Point_i type) {
	if (type == Sizing2::Left) return SIZING_LEFT;
	if (type == Sizing2::Right) return SIZING_RIGHT;
	if (type == Sizing2::Bottom) return SIZING_BOTTOM;
	if (type == Sizing2::Top) return SIZING_TOP;

	if (type == Sizing2::BottomLeft) return SIZING_BOTTOM_LEFT;
	if (type == Sizing2::BottomRight) return SIZING_BOTTOM_RIGHT;
	if (type == Sizing2::TopLeft) return SIZING_TOP_LEFT;
	if (type == Sizing2::TopRight) return SIZING_TOP_RIGHT;

	return SIZING_RESTORED;
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
	/*if (rect.x() == min.x && arect.x() != amin.x) {
		rect.ax++;
		rect.bx++;
		updateAlpha;
	}
	if (rect.x() == max.x && arect.x() != amax.x) {
		rect.ax--;
		rect.bx--;
		updateAlpha;
	}*/

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
			double& alpha, double& aalpha) {
	// Два окна всегда должны быть одной высоты и на одной высоте
	arect.ay = rect.ay;
	arect.by = arect.ay + rect.y();

	// Рассматриваем два типа ресайзинга левого окна
	if (type.x < 0)
		rLeftSide(rect, arect, min, max, amin, amax, alpha, aalpha);
	else if (type.x > 0)
		rRightSide(rect, arect, min, max, amin, amax, alpha, aalpha);

	if (type.y > 0)
		rUpSide(rect, arect, min, max, amin, amax);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void WindowCarcass::assign(CarcassedWindow* _wnd1, 
						   CarcassedWindow* _wnd2, 
						   bool _isVertical) {
	isSetRect = false;
	parent = nullptr;
	isFirst = false;

	wnd1 = _wnd1;
	wnd2 = _wnd2;

	notResize = false;
	anotResize = false;
	isVertical = _isVertical;

	min = wnd1->c_getMinSize();
	max = wnd1->c_getMaxSize();
	amin = wnd2->c_getMinSize();
	amax = wnd2->c_getMaxSize();

	rect = wnd1->c_getRect();
	arect = wnd2->c_getRect();

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

	wnd1->isFirst = true;
	wnd2->isFirst = false;
	wnd1->parent = this;
	wnd2->parent = this;

	wnd1->c_setRect(rect);
	wnd2->c_setRect(arect);
}

//-----------------------------------------------------------------------------
void  resizeWnd(bool& notResize, bool& anotResize,
				bool isFirst, bool isVertical,
				Point_i type,
				Rect& rect, Rect& arect,
				Point_i min, Point_i max,
				Point_i amin, Point_i amax,
				double& alpha, double& aalpha,
				Rect& origRect) {
	bool isBottom = (type.y < 0 && !isVertical) || (type.x > 0 && isVertical);
	if (!notResize) {
		anotResize = true;

		// Если два окна вертикальные, то меняем координаты так, чтобы они стали горизонатльными
		if (isVertical) {
			changeXY(type);
			changeY(type);
			changeX(type);
			changeXY(rect);
			changeXY(arect);
			changeXY(min);
			changeXY(max);
			changeXY(amin);
			changeXY(amax);
		}

		// Если это правое окно, преобразуем координаты так, чтобы оно стало левым
		if (!isFirst) {
			changeX(type);
			changeX(rect);
			changeX(arect);
		}

		// Если ресайзится низ то изменяем координаты так, чтобы низ стал верхом
		if (isBottom) {
			changeY(type);
			changeY(rect);
			changeY(arect);
		}

		// Рассчитываем ресайзинг левого окна горизонтального каркаса окон, где может ресайзиться только левая, правая и верхняя сторона
		::resize(type, 
				 rect, arect, 
				 min, max, 
				 amin, amax, 
				 alpha, aalpha);

		// Меняем местами обратно верх с низом
		if (isBottom) {
			changeY(type);
			changeY(rect);
			changeY(arect);
		}

		// Если это правое окно, возвращаем координаты так, чтобы оно снова было правым
		if (!isFirst) {
			changeX(type);
			changeX(rect);
			changeX(arect);
		}

		// Возвращаем горизонтальные координаты снова к вертикальным, если это вертикальные окна
		if (isVertical) {
			changeXY(type);
			changeY(type);
			changeX(type);
			changeXY(rect);
			changeXY(arect);
			changeXY(min);
			changeXY(max);
			changeXY(amin);
			changeXY(amax);
		}

		origRect = rect;
	}
	notResize = false;
}

//-----------------------------------------------------------------------------
void* WindowCarcass::c_sendMessageUp(int32u messageNo, void* data) {
	if (messageNo == CARCASS_RESIZE) {
		WhenResize* when = (WhenResize*)data;
		Point_i type2 = sizing2sizing2(when->type);
		if (when->isFirst) {
			resizeWnd(notResize, anotResize,
					  true, isVertical,
					  type2,
					  when->rect, arect,
					  min, max, amin, amax,
					  alpha, aalpha,
					  rect);
			bool haveParent = parent != nullptr;
			bool isNotInsideResize = ((type2.x < 0 || type2.y != 0) && !isVertical) ||
				((type2.y > 0 || type2.x != 0) && isVertical);
			if (haveParent && !isSetRect && isNotInsideResize) {
				if (isVertical)
					if (type2.y < 0) type2.y = 0;
					else ;
				else
					if (type2.x > 0) type2.x = 0;
					else ;
				SizingType type = sizing22sizing(type2);
				Rect newRect = c_getRect();
				WhenResize when = {isFirst, newRect, type};
				parent->c_sendMessageUp(CARCASS_RESIZE, &when);
			} else {
				wnd1->c_setRect(rect);
				wnd2->c_setRect(arect);
			}
		} else {
			resizeWnd(anotResize, notResize,
					  false, isVertical,
					  sizing2sizing2(when->type),
					  when->rect, rect,
					  amin, amax, min, max,
					  aalpha, alpha,
					  arect);
			bool haveParent = parent != nullptr;
			bool isNotInsideResize = ((type2.x > 0 || type2.y != 0) && !isVertical) ||
				((type2.y < 0 || type2.x != 0) && isVertical);
			if (haveParent && !isSetRect && isNotInsideResize) {
				if (isVertical)
					if (type2.y > 0) type2.y = 0;
					else ;
				else
					if (type2.x < 0) type2.x = 0;
					else ;
				SizingType type = sizing22sizing(type2);
				Rect newRect = c_getRect();
				WhenResize when = {isFirst, newRect, type};
				parent->c_sendMessageUp(CARCASS_RESIZE, &when);
			} else {
				wnd1->c_setRect(rect);
				wnd2->c_setRect(arect);
			}
		}
	} else 
	if (messageNo == CARCASS_MOVE) {
		WhenMove* when = (WhenMove*)data;
		if (when->isFirst) {
			anotResize = true;
			Point_i diff(when->newPos.x - rect.ax, when->newPos.y - rect.ay);
			rect.ax += diff.x;
			rect.ay += diff.y;
			rect.bx += diff.x;
			rect.by += diff.y;
			arect.ax += diff.x;
			arect.ay += diff.y;
			arect.bx += diff.x;
			arect.by += diff.y;
			wnd2->c_setRect(arect);
			bool haveParent = parent != nullptr;
			if (haveParent && !isSetRect) {
				Rect newRect = c_getRect();
				WhenMove when = {isFirst, Point_i(newRect.ax, newRect.ay)};
				parent->c_sendMessageUp(CARCASS_MOVE, &when);
			}
		} else {
			notResize = true;
			Point_i diff(when->newPos.x - arect.ax, when->newPos.y - arect.ay);
			rect.ax += diff.x;
			rect.ay += diff.y;
			rect.bx += diff.x;
			rect.by += diff.y;
			arect.ax += diff.x;
			arect.ay += diff.y;
			arect.bx += diff.x;
			arect.by += diff.y;
			wnd1->c_setRect(rect);
			bool haveParent = parent != nullptr;
			if (haveParent && !isSetRect) {
				Rect newRect = c_getRect();
				WhenMove when = {isFirst, Point_i(newRect.ax, newRect.ay)};
				parent->c_sendMessageUp(CARCASS_MOVE, &when);
			}
		}
	}
	return nullptr;
}

//-----------------------------------------------------------------------------
void WindowCarcass::c_waitForClose(bool isAllClosed) {
	if (isAllClosed) {
		wnd1->c_waitForClose(true);
		wnd2->c_waitForClose(true);
	} else {
		while (!wnd1->c_isClosed() && !wnd2->c_isClosed())
			sleep(10);
	}
}

//-----------------------------------------------------------------------------
bool WindowCarcass::c_isClosed() {
	return wnd1->c_isClosed() || wnd2->c_isClosed();
}

//-----------------------------------------------------------------------------
Rect WindowCarcass::c_getRect(void) {
	Rect orect;
	orect.ax = rect.ax;
	orect.ay = rect.ay;
	orect.bx = arect.bx;
	orect.by = arect.by;
	return orect;
}

//-----------------------------------------------------------------------------
Point_i WindowCarcass::c_getMaxSize(void) {
	if (isVertical)
		return Point_i(TWG_min(max.x, amax.x), max.y + amax.y);
	else 
		return Point_i(max.x + amax.x, TWG_min(max.y, amax.y));
}

//-----------------------------------------------------------------------------
Point_i WindowCarcass::c_getMinSize(void) {
	if (isVertical)
		return Point_i(TWG_max(min.x, amin.x), min.y + amin.y);
	else 
		return Point_i(min.x + amin.x, TWG_max(min.y, amin.y));
}

//-----------------------------------------------------------------------------
void WindowCarcass::c_setRect(Rect rect1) {
	Point_i omin = c_getMinSize();
	Point_i omax = c_getMaxSize();
	if (rect1.x() < omin.x) rect1.bx = rect1.ax + omin.x;
	if (rect1.y() < omin.y) rect1.by = rect1.ay + omin.y;
	if (rect1.x() > omax.x) rect1.bx = rect1.ax + omax.x;
	if (rect1.y() > omax.y) rect1.by = rect1.ay + omax.y;

	if (isVertical) {
		changeXY(rect);
		changeXY(arect);
		changeXY(rect1);
		changeXY(min);
		changeXY(max);
		changeXY(amin);
		changeXY(amax);
	}

	rect.ax = rect1.ax;
	rect.ay = rect1.ay;
	rect.by = rect1.by;
	arect.ay = rect1.ay;
	arect.bx = rect1.bx;
	arect.by = rect1.by;

	rLeftSide(rect, arect, min, max, amin, amax, alpha, aalpha);

	if (isVertical) {
		changeXY(rect);
		changeXY(arect);
		changeXY(rect1);
		changeXY(min);
		changeXY(max);
		changeXY(amin);
		changeXY(amax);
	}

	isSetRect = true;

	wnd1->c_setRect(rect);
	wnd2->c_setRect(arect);

	isSetRect = false;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void test1() {
	WindowType type(-1, Point_i(100, 100), Point_i(100, 100), Point_i(50, 50), Point_i(-1, -1), L"Window", WindowStyle(WINDOW_TOOL | WINDOW_ON_TOP));

	WindowCarcass wndc1;
	CarcassTemplate<WindowEvents> wnd1(type), wnd2(type);
	wndc1.assign(&wnd1, &wnd2, true);

	wndc1.c_waitForClose(false);
}

//-----------------------------------------------------------------------------
void test2() {
	WindowType type(-1, Point_i(100, 100), Point_i(100, 100), Point_i(50, 50), Point_i(-1, -1), L"Window", WindowStyle(WINDOW_TOOL | WINDOW_ON_TOP));

	WindowCarcass wndc1;
	CarcassTemplate<WindowEvents> wnd1(type), wnd2(type);
	wndc1.assign(&wnd1, &wnd2, false);

	wndc1.c_waitForClose(false);
}

//-----------------------------------------------------------------------------
void test3() {
	WindowType type(-1, Point_i(100, 100), Point_i(100, 100), Point_i(50, 50), Point_i(-1, -1), L"Window", WindowStyle(WINDOW_TOOL | WINDOW_ON_TOP));

	WindowCarcass wndc1, wndc2;
	CarcassTemplate<WindowEvents> wnd1(type), wnd2(type), wnd3(type);
	wndc1.assign(&wnd1, &wnd2, true);
	wndc2.assign(&wndc1, &wnd3, false);

	wndc2.c_waitForClose(false);
}

//-----------------------------------------------------------------------------
void test4() {
	WindowType type(-1, Point_i(100, 100), Point_i(100, 100), Point_i(50, 50), Point_i(-1, -1), L"Window", WindowStyle(WINDOW_TOOL | WINDOW_ON_TOP));

	WindowCarcass wndc1, wndc2, wndc3;
	CarcassTemplate<WindowEvents> wnd1(type), wnd2(type), wnd3(type), wnd4(type);

	wndc1.assign(&wnd1, &wnd2, true);
	wndc2.assign(&wnd3, &wndc1, false);
	wndc3.assign(&wndc2, &wnd4, false);

	wndc3.c_waitForClose(false);
}

//-----------------------------------------------------------------------------
void test5() {
	WindowType type1(-1,
		Point_i(100, 100),
		Point_i(100, 100), 
		Point_i(50, 40),
		Point_i(200, 150),
		L"Left Up",
		WindowStyle(WINDOW_TOOL | WINDOW_ON_TOP));
	WindowType type2(-1,
		Point_i(100, 100),
		Point_i(100, 100), 
		Point_i(50, 50),
		Point_i(200, 110),
		L"Left Down",
		WindowStyle(WINDOW_TOOL | WINDOW_ON_TOP));
	WindowType type3(-1,
		Point_i(100, 100),
		Point_i(100, 100), 
		Point_i(50, 50),
		Point_i(200, 400),
		L"Right",
		WindowStyle(WINDOW_TOOL | WINDOW_ON_TOP));

	WindowCarcass wndc1, wndc2;
	CarcassTemplate<WindowEvents> wnd1(type1), wnd2(type2), wnd3(type3);

	wndc1.assign(&wnd1, &wnd2, true);
	wndc2.assign(&wnd3, &wndc1, false);

	wndc2.c_waitForClose(false);
}

//-----------------------------------------------------------------------------
void test6() {
	WindowType type(-1, Point_i(100, 100), Point_i(100, 100), Point_i(50, 50), Point_i(-1, -1), L"Window", WindowStyle(WINDOW_TOOL | WINDOW_ON_TOP));

	WindowCarcass wndc1, wndc2, wndc3, wndc4, wndc5;
	CarcassTemplate<WindowEvents> wnd1(type), wnd2(type), wnd3(type), wnd4(type), wnd5(type), wnd6(type);

	wndc1.assign(&wnd1, &wnd2, true);
	wndc2.assign(&wnd3, &wnd4, true);
	wndc3.assign(&wnd5, &wnd6, false);

	wndc4.assign(&wndc1, &wndc2, false);
	wndc5.assign(&wndc4, &wndc3, true);

	wndc5.c_waitForClose(false);
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//int main() {
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	test6();
}