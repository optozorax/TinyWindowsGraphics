#include "twg/window.h"

namespace twg
{

//-----------------------------------------------------------------------------
WindowBase::WindowBase(WindowType type) {

}
//-----------------------------------------------------------------------------
WindowBase::~WindowBase() {

}

//-----------------------------------------------------------------------------
Point_i WindowBase::getClientSize(void) {

}

//-----------------------------------------------------------------------------
Point_i WindowBase::getWindowSize(void) {

}

//-----------------------------------------------------------------------------
Point_i WindowBase::getPos(void) {

}

//-----------------------------------------------------------------------------
Point_i WindowBase::getMaxSize(void) {

}

//-----------------------------------------------------------------------------
Point_i WindowBase::getMinSize(void) {

}

//-----------------------------------------------------------------------------
std::string WindowBase::getCaption(void) {

}

//-----------------------------------------------------------------------------
WindowStyle WindowBase::getStyle(void) {

}

//-----------------------------------------------------------------------------
void WindowBase::setClientSize(Point_i size) {

}

//-----------------------------------------------------------------------------
void WindowBase::setWindowSize(Point_i size) {

}

//-----------------------------------------------------------------------------
void WindowBase::setPos(Point_i size) {

}

//-----------------------------------------------------------------------------
void WindowBase::setMaxSize(Point_i maxSize) {

}

//-----------------------------------------------------------------------------
void WindowBase::setMinSize(Point_i minSize) {

}

//-----------------------------------------------------------------------------
void WindowBase::setCaption(std::string caption) {

}

//-----------------------------------------------------------------------------
void WindowBase::setStyle(WindowStyle style) {

}

//-----------------------------------------------------------------------------
void WindowBase::invalidateScreen(void) {

}

//-----------------------------------------------------------------------------
HWND WindowBase::create(void*) {

}

//-----------------------------------------------------------------------------
LRESULT WindowBase::wndProc(HWND hwnd, 
							UINT msg,
							WPARAM wParam, 
							LPARAM lParam) {

}

//-----------------------------------------------------------------------------
LRESULT WindowBase::wndProcNext(HWND hwnd, 
								UINT msg,
								WPARAM wParam, 
								LPARAM lParam) {

}

}