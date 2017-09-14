#include <sstream>
#include <twg/twg.h>
#include <twg/window/window_ctrl.h>
#include <twg/image/image_drawing.h>
#include <twg/ctrl/clickable_ctrl.h>

using namespace twg;

//-----------------------------------------------------------------------------
class Button : public ClickableCtrl
{
public:
	Button(Rect rect, std::wstring str, EventsBase* parent) : ClickableCtrl(parent), rect(rect), str(str){}

private:
	Rect	rect;
	std::wstring str;

	bool isInside(Point_i pos);
	void onClick(void);

	void drawDefault(ImageBase* buffer);
	void drawHover(ImageBase* buffer);
	void drawWhenClick(ImageBase* buffer);
};

//=============================================================================
//=============================================================================
//=============================================================================

//-----------------------------------------------------------------------------
bool Button::isInside(Point_i pos) {
	return pos.inRectangle(Point_i(rect.ax, rect.ay), Point_i(rect.bx, rect.by));
}

//-----------------------------------------------------------------------------
void Button::onClick(void) {
	static int32u count = 0;
	count++;
	std::wstringstream sout;
	sout << L"You clicked " << count << L" times";
	m_wnd->setCaption(sout.str());
}

//-----------------------------------------------------------------------------
void Button::drawDefault(ImageBase* buffer) {
	ImageDrawing_aa img(buffer);
	
	for (int32 j = rect.ay; j < rect.by; ++j) {
		Color clr = getColorBetween(double(j-rect.ay)/rect.y(), 
			White,
			rgb(0xed, 0xed, 0xed));
		for (int32 i = rect.ax; i < rect.bx; ++i)
			img[Point_i(i, j)] = clr;
	}

	Polygon_d poly;
	poly.array.push_back(Point_d(rect.ax, rect.ay));
	poly.array.push_back(Point_d(rect.bx, rect.ay));
	poly.array.push_back(Point_d(rect.bx, rect.by));
	poly.array.push_back(Point_d(rect.ax, rect.by));

	img.setPen(Pen(0.5, Gray));
	img.drawPolyline(poly);

	img.setPen(Pen(1, Gray));
	img.setTextStyle(TextStyle(14, L"Consolas", TEXT_NONE));
	Point_d textSize(img.getTextSize(str));
	Point_d pos((poly.array[0] + poly.array[2])/2 - Point_i(textSize.x, -textSize.y)/2 - Point_i(0, 3));
	img.drawText(pos, str);
}

//-----------------------------------------------------------------------------
void Button::drawHover(ImageBase* buffer) {
	ImageDrawing_aa img(buffer);

	for (int32 j = rect.ay; j < rect.by; ++j) {
		Color clr = getColorBetween(double(j-rect.ay)/rect.y(), 
			White,
			rgb(0xdc, 0xdc, 0xdc));
		for (int32 i = rect.ax; i < rect.bx; ++i)
			img[Point_i(i, j)] = clr;
	}

	Polygon_d poly;
	poly.array.push_back(Point_d(rect.ax, rect.ay));
	poly.array.push_back(Point_d(rect.bx, rect.ay));
	poly.array.push_back(Point_d(rect.bx, rect.by));
	poly.array.push_back(Point_d(rect.ax, rect.by));

	img.setPen(Pen(0.5, Gray));
	img.drawPolyline(poly);

	img.setPen(Pen(1, Gray));
	img.setTextStyle(TextStyle(14, L"Consolas", TEXT_NONE));
	Point_d textSize(img.getTextSize(str));
	Point_d pos((poly.array[0] + poly.array[2])/2 - Point_i(textSize.x, -textSize.y)/2 - Point_i(0, 3));
	img.drawText(pos, str);
}

//-----------------------------------------------------------------------------
void Button::drawWhenClick(ImageBase* buffer) {
	rect.ay += 2;
	rect.by += 2;
	ImageDrawing_aa img(buffer);

	for (int32 j = rect.ay; j < rect.by; ++j) {
		Color clr = getColorBetween(double(j-rect.ay)/rect.y(), 
			rgb(0xed, 0xed, 0xed),
			White);
		for (int32 i = rect.ax; i < rect.bx; ++i)
			img[Point_i(i, j)] = clr;
	}

	Polygon_d poly;
	poly.array.push_back(Point_d(rect.ax, rect.ay));
	poly.array.push_back(Point_d(rect.bx, rect.ay));
	poly.array.push_back(Point_d(rect.bx, rect.by));
	poly.array.push_back(Point_d(rect.ax, rect.by));

	img.setPen(Pen(0.5, Gray));
	img.drawPolyline(poly);

	img.setPen(Pen(1, Gray));
	img.setTextStyle(TextStyle(14, L"Consolas", TEXT_NONE));
	Point_d textSize(img.getTextSize(str));
	Point_d pos((poly.array[0] + poly.array[2])/2 - Point_i(textSize.x, -textSize.y)/2 - Point_i(0, 3));
	img.drawText(pos, str);

	rect.ay -= 2;
	rect.by -= 2;
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

	Point_i winSize = wnd.getClientSize();
	Point_i size(70, 30);
	Point_i a(winSize/2 - size/2);
	Point_i b(winSize/2 + size/2);
	Rect rect(a.x, a.y, b.x, b.y);
	wnd.storage.array.push_back(new Button(rect, L"Button", &wnd.storage));
	wnd.redraw();

	wnd.waitForClose();
}