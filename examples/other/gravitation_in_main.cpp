#include <twg/twg.h>
#include <twg/image/image_drawing.h>

using namespace twg;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	/*  Использованные физические формулы: 
		F = G*m1*m2/r^2 (в векторном виде: F = G*m1*m2/|r|^2 * (r/|r|); где 
			F - вектор силы, r - вектор направленый из текущего тела к другому
			и по модулю равный расстоянию между телами.)
		Формулы далее справедливы как в векторном, так и в скалярном виде:
		a = F/m
		v' = v + a*t
		x' = x + v*t
	 */
	const Point_i size(400, 400);		// Размер изображения
	const double  G = 0.01;				// Гравитационная постоянная
	const double  dt = 0.01;			// Время одной итерации
	const Point_d sunPos = size / 2.0;	// Положение солнца
	const double  sunMass = 20000;		// Масса солнца
	const int32u  redrawCount = 150;	// Количество итераций, после которых изображение перерисуется

	WindowType type(stdIcon, 
		Point_i(100, 100),
		size,
		stdMin, stdMax,
		L"Simple gravitation simulation",
		WINDOW_STANDART);
	WindowBase wnd(type);
	wnd.setClientSize(size);

	// Изображение, в которое будет происходить рисование
	ImageDrawing_aa img(size);

	// Начальные параметры точки
	Point_d pointPos(size.x/2.0, 30);
	Point_d pointVelocity(0.8, 0);
	double  pointMass = 2;

	int32u iterations = 0;
	// Пока не закроют окно, продолжаем итерации.
	while (!wnd.isClosed()) {		
		// Симулируем необходимое количество итераций для перерисовки
		for (int32 i = 0; i < redrawCount; i++) {
			Point_d pointForce(0, 0);
			pointForce = sunPos-pointPos; // Направление силы всемирного тяготения
			pointForce *= G*sunMass*pointMass/pow(pointForce.getLength(), 3); // Расстояние возводится в куб, чтобы длину вектора pointForce уменьшить до единицы. Чтобы единичный вектор направления умножался на величину силы.
			pointVelocity += pointForce/pointMass*dt;
			pointPos += pointVelocity*dt;
		}

		img.clear(Black); // Если закомментировать эту строчку, то будет рисоваться путь точки

		img.setBrush(Brush(setAlpha(White, 128)));
		img.drawPolygon(computeEllipse(Point_d(4, 4)).move(pointPos));

		img.setBrush(Brush(Yellow));
		img.drawPolygon(computeEllipse(Point_d(10, 10)).move(sunPos));

		// Рисуем внутреннее изображение на экран
		img.copyTo(&wnd.canvas, Point_i(0, 0), Point_i(0, 0), img.size());

		sleep(10);
	}
}