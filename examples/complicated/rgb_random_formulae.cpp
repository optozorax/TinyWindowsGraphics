#include <twg/twg.h>
#include <twg/window/window_ctrl.h>
#include <twg/ctrl/menu.h>

using namespace twg;

class RandomFormulae
{
public:
	RandomFormulae() {}

	void generate(int8u maxDepth);
	int8u calculate(int8u x, int8u y);
	std::wstring write(void);
private:
	std::vector<int8> 	m_mas;
	int32u 				m_pos;

	const int8u opBinCount = 8;
	const int8u opUnCount = 4;
	const double unarPercent = 0.50;
	const double binarPercent = 0.70;

	int8u r_calculate(int8u x, int8u y);
	void r_generate(int8u maxDepth);
	std::wstring r_write(void);
};

//-----------------------------------------------------------------------------
class MyMenu : public StaticMenu
{
public:
	MyMenu(std::wstring str, EventsBase* parent) : StaticMenu(str, parent), img(Point_i(256, 256)), maxDepth(2) {}

	void onClick(int32u id);
	void draw(ImageBase* buffer);

	int8u maxDepth;
	ImageBase img;
	RandomFormulae f;
};

//=============================================================================
//=============================================================================
//=============================================================================

//-----------------------------------------------------------------------------
int8u mod(int8u x, int8u y) {
	if (y != 0)
		return x % y;
	else
		return 0;
}

//-----------------------------------------------------------------------------
int8u div(int8u x, int8u y) {
	if (y != 0)
		return x / y;
	else
		return 0;
}

//-----------------------------------------------------------------------------
int8u usin(int8u x) {
	return (sin(x * pi/256) + 1) * 128;
}

//-----------------------------------------------------------------------------
int8u ucos(int8u x) {
	return (cos(x * pi/256) + 1) * 128;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void RandomFormulae::generate(int8u maxDepth) {
	m_pos = 0;
	m_mas.erase(m_mas.begin(), m_mas.end());
	r_generate(maxDepth);
}

//-----------------------------------------------------------------------------
int8u RandomFormulae::calculate(int8u x, int8u y) {
	m_pos = 0;
	return r_calculate(x, y);
}

//-----------------------------------------------------------------------------
std::wstring RandomFormulae::write(void) {
	m_pos = 0;
	return r_write();
}

//-----------------------------------------------------------------------------
int8u RandomFormulae::r_calculate(int8u x, int8u y) {
	if (m_mas[m_pos] < 0 && m_mas[m_pos] > -3) {
		switch (m_mas[m_pos]) {
			case -2: return y;
			case -1: return x;
		}	
	} else 
	if (m_mas[m_pos] < 0) {
		int8 op = m_mas[m_pos]; 
		m_pos++;
		int8u x1 = r_calculate(x, y);
		switch (op) {
			case -3: return sqrt(x1);
			case -4: return usin(x1);
			case -5: return ucos(x1);
			case -6: return ~x1;
		}	
	} else {
		int8u op = m_mas[m_pos]; 
		m_pos++;
		int8u x1 = r_calculate(x, y);
		m_pos++;
		int8u y1 = r_calculate(x, y);
		switch (op) {
			case 1: return x1 + y1;
			case 2: return x1 - y1;
			case 3: return x1 * y1;
			case 4: return div(x1, y1);
			case 5: return mod(x1, y1);
			case 6: return x1 & y1;
			case 7: return x1 | y1;
			case 8: return x1 ^ y1;
		}
	}
}

//-----------------------------------------------------------------------------
void RandomFormulae::r_generate(int8u maxDepth) {
	int32u ran = std::rand();
	if (maxDepth != 0) {
		if (ran/double(RAND_MAX) < binarPercent) {
			ran = std::rand();
			m_mas.push_back(ran % opBinCount + 1);
			r_generate(maxDepth-1);
			r_generate(maxDepth-1);
		} else {
			ran = std::rand();
			if (ran/double(RAND_MAX) < unarPercent) {
				ran = std::rand();
				m_mas.push_back(-(ran % opUnCount + 1) - 2);
				r_generate(maxDepth-1);
			} else {
				m_mas.push_back(-(ran % 2 + 1));
			}
		}
	} else {
		m_mas.push_back(-(ran % 2 + 1));
	}
}

//-----------------------------------------------------------------------------
std::wstring RandomFormulae::r_write(void) {
	if (m_mas[m_pos] < 0 && m_mas[m_pos] > -3) {
		switch (m_mas[m_pos]) {
			case -2: return L"y";
			case -1: return L"x";
		}	
	} else 
	if (m_mas[m_pos] < 0) {
		int8 op = m_mas[m_pos]; 
		bool brackets = false;
		m_pos++;
		brackets  = !(m_mas[m_pos] == -1 || m_mas[m_pos] == -2);
		std::wstring x1 = r_write();
		if (brackets) x1 = L"(" + x1 + L")";
		switch (op) {
			case -3: return L"sqrt(" + x1 + L")";
			case -4: return L"usin(" + x1 + L")";
			case -5: return L"ucos(" + x1 + L")";
			case -6: return L"not(" + x1 + L")";
		}	
	} else {
		int8u op = m_mas[m_pos];
		bool brackets = false;
		m_pos++;
		brackets  = !(m_mas[m_pos] == -1 || m_mas[m_pos] == -2);
		std::wstring x1 = r_write();
		if (brackets) x1 = L"(" + x1 + L")";
		m_pos++;
		brackets  = !(m_mas[m_pos] == -1 || m_mas[m_pos] == -2);
		std::wstring y1 = r_write();
		if (brackets) y1 = L"(" + y1 + L")";
		switch (op) {
			case 1: return x1 + L" + " + y1;
			case 2: return x1 + L" - " + y1;
			case 3: return x1 + L" mul " + y1;
			case 4: return L"div(" + x1 + L", " + y1 + L")";
			case 5: return L"mod(" + x1 + L", " + y1 + L")";
			case 6: return x1 + L" and " + y1;
			case 7: return x1 + L" or " + y1;
			case 8: return x1 + L" xor " + y1;
		}
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void MyMenu::onClick(int32u id) {
	switch (id) {
		case 1:
			f.generate(maxDepth);
			m_wnd->setCaption(f.write());
			for (int32u i = 0; i < 256; ++i) {
				for (int32u j = 0; j < 256; ++j) {
					int8u clr = f.calculate(i, j);
					img[Point_i(i, j)] = rgb(clr, clr, clr);
				}
			}
			m_wnd->worthRedraw();
			break;
		case 2:
			if (maxDepth < 255)
				maxDepth++;
			break;
		case 3: 
			if (maxDepth > 1)
				maxDepth--;
			break;
		case 4:
			saveToBmp(&img, f.write() + L".bmp");
	}
}

void MyMenu::draw(ImageBase* buffer) {
	img.copyTo(buffer, Point_i(0, 0), Point_i(0, 0), img.size());
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//int main() {
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	std::srand(timeGet());
	WindowType type(stdIcon,
		Point_i(100, 100),
		Point_i(300, 300),
		stdMin, stdMax,
		L"Generate a random rgb formulae",
		WINDOW_STANDART);
	WindowCtrl wnd(type);

	MyMenu* menu = new MyMenu(L"=1 Generate | =2 Depth++ | =3 Depth-- | =4 Save", &wnd.storage);
	wnd.storage.array.push_back(menu);
	menu->onClick(1);

	wnd.setClientSize(Point_i(256, 256));
	wnd.setMaxSize(wnd.getWindowSize());
	wnd.setMinSize(wnd.getWindowSize());

	wnd.waitForClose();
}