#include <sstream>
#include "twg/twg.h"

using namespace twg;

void test1(void) {
	// ImageWin test
	// saveToBmp test
	// Save screenshoot
	ImageWin screen;
	screen.assignScreen();

	ImageBase img(screen.size());
	screen.copyTo(&img, Point_i(0, 0), Point_i(0, 0), screen.size());

	saveToBmp(&img, L"test1.bmp");
}

void test2(void) {
	ImageBase img(Point_i(1, 1));
	ImageBase img2(Point_i(100, 100));

	img.resize(Point_i(200, 200));
	img.clear(Miku);

	img2.clear(setRed(Miku, 0));
	img2.copyTo(&img, Point_i(0, 0), Point_i(0, 0), img2.size());

	img2.clear(setGreen(Miku, 0));
	img2.copyTo(&img, Point_i(0, 100), Point_i(0, 0), img2.size());

	img2.clear(setBlue(Miku, 0));
	img2.copyTo(&img, Point_i(100, 0), Point_i(0, 0), img2.size());

	saveToBmp(&img, L"test2.bmp");
}

void test3(void) {
	#define writeColor(A) \
		   #A L" = rgba(" \
		<< getRed(A) << L", " \
		<< getGreen(A) << L", " \
		<< getBlue(A) << L", " \
		<< getAlpha(A) << L");"

	std::wstringstream sout;

	sout << L"Цвет " << writeColor(Miku) << std::endl;
	sout << L"Цвет " << writeColor(setAlpha(Miku, 0)) << std::endl;
	sout << L"Цвет " << writeColor(setRed(Miku, 0)) << std::endl;
	sout << L"Цвет " << writeColor(setGreen(Miku, 0)) << std::endl;
	sout << L"Цвет " << writeColor(setBlue(Miku, 0)) << std::endl;
	
	messageBox(sout.str(), std::wstring(L"Тест цвета."), MESSAGE_OK | MESSAGE_ICON_INFORMATION);

}

void test4(void) {
	// Test of getColorBetween
	// Test of getGrayHue
	// Test of computeCycle
	// Test of getRainbow
	// Test of getGradient
	// Test of computeOverlay
	ImageBase img(Point_i(256, 256));

	Color clr;
	for (int i = 0; i < img.width(); ++i) {
		clr = getColorBetween(i/256.0, Miku, Bitcoin);
		for (int j = 0; j < 64; ++j)
			img[Point_i(i, j)] = clr;
	}

	for (int i = 0; i < img.width(); ++i) {
		clr = getGrayHue(computeCycle(i/64.0, CYCLE_SIN));
		for (int j = 64; j < 96; ++j)
			img[Point_i(i, j)] = clr;
	}

	for (int i = 0; i < img.width(); ++i) {
		clr = getGrayHue(computeCycle(i/64.0, CYCLE_TAN));
		for (int j = 96; j < 128; ++j)
			img[Point_i(i, j)] = clr;
	}

	for (int i = 0; i < img.width(); ++i) {
		clr = getRainbow(i/256.0);
		for (int j = 128; j < 160; ++j)
			img[Point_i(i, j)] = clr;
	}

	for (int i = 0; i < img.width(); ++i) {
		clr = computeOverlay(getColorBetween(i/256.0, Black, Transparent), 
							 getRainbow(i/256.0));
		for (int j = 160; j < 192; ++j)
			img[Point_i(i, j)] = clr;
	}

	std::vector<Color> clrs;
	clrs.push_back(White);
	clrs.push_back(Blue);
	clrs.push_back(Red);
	std::vector<double> sizes;
	sizes.push_back(0.5);
	sizes.push_back(1);
	for (int i = 0; i < img.width(); ++i) {
		clr = getGradient(i/256.0, clrs, sizes);
		for (int j = 192; j < 256; ++j)
			img[Point_i(i, j)] = clr;
	}

	saveToBmp(&img, L"test4.bmp");
}

void test5() {
	// Test of PointIterator
	// Test of computeOverlay
	ImageBase img(Point_i(101, 101));
	img.clear(White);

	Point_i size(20, 20);
	int32u count = 10;
	for (int k = 0; k <= count; ++k) {
		Color clr;
		if (k % 2 == 1) 
			clr = setAlpha(Red, 128);
		else 
			clr = setAlpha(Aqua, 128);


		Point_i start = (img.size() - size)/count * k;
		PointIterator<Point_i> i(start, start + size);
		for (i.onStart(); !i.isEnd(); ++i) {
			img[*i] = computeOverlay(clr, img[*i]);
		}
	}

	saveToBmp(&img, L"test5.bmp");
}

int main() {
	test1();
	test2();
	test3();
	test4();
	test5();

	messageBox(std::wstring(L"Это тестирование классов ImageWin и ImageBase, а так же функций сохранения изображения и методов для цвета.\nВ результате работы программы были созданы изображения:\n\ttest1.bmp\n\ttest2.bmp\n\ttest4.bmp\n\ttest5.bmp"), std::wstring(L"Тест изображений"), MESSAGE_OK | MESSAGE_ICON_INFORMATION);
	return 0;
}