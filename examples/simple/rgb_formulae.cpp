#include <twg/image.h>
#include <string>

using namespace twg;

//-----------------------------------------------------------------------------
template<class Formula>
void makeImage(int8u num, Formula f) {
	ImageBase img(Point_i(256, 256));

	for (int32u i = 0; i < 256; ++i) {
		for (int32u j = 0; j < 256; ++j) {
			img[Point_i(i, j)] = rgb(f(i, j), f(i, j), f(i, j));
		}
	}

	std::string str = std::to_string(num);
	saveToBmp(&img, L"rgb_" + std::wstring(str.begin(), str.end()) + L".bmp");
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

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

int main() {
	/* Пример вычисления цвета пикселя в зависимости от его положения. */

	int8u i = 0;

	//-------------------------------------------------------------------------

	ImageBase img(Point_i(256, 256));

	for (int32u i = 0; i < 256; ++i) {
		for (int32u j = 0; j < 256; ++j) {
			img[Point_i(i, j)] = rgb(i + j, i * j, i ^ j);
		}
	}

	std::string str = std::to_string(++i);
	saveToBmp(&img, L"rgb_" + std::wstring(str.begin(), str.end()) + L".bmp");

	//-------------------------------------------------------------------------
	// Далее идет тоже самое, только более кратко записывается основная часть выражения при помощи лямбда выражений и шаблонной функции. Так же без цвета(всем каналам задается одно и тоже значение), чтобы сосредоточиться на содержании формул. А так можно как угодно комбинировать величину синего, красного и зеленого при помощи этих формул.

	//-------------------------------------------------------------------------
	// Базовые вычисления
	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return x + y;
	});
	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return x * y;
	});
	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return mod(x, y);
	});
	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return div(x, y);
	});
	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return x & y;
	});
	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return x | y;
	});
	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return x ^ y;
	});
	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return sqrt(x);
	});
	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return ~x;
	});
	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return usin(x);
	});
	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return ucos(x);
	});

	//-------------------------------------------------------------------------
	// Комбинации базовых вычислений
	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return mod(x, x & y);
	});

	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return mod(x, x | y);
	});

	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return mod(x, x ^ y);
	});

	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return mod(x & y, y);
	});

	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return mod(x | y, y);
	});

	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return mod(x ^ y, y);
	});

	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return mod(mod(x, x & y), mod(x & y, y));
	});

	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return x*x + y*y;
	});

	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return x*x*x + y*y*y;
	});

	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return x*x & y*y;
	});

	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return (x & y) * (x & y);
	});

	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return mod(x, y) * mod(x, y);
	});

	//-------------------------------------------------------------------------
	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return (((y ^ x) + (x | y)) | x) | (((x - y) ^ x) & ((x + y) - (x & y)));
	});
	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return (((y ^ x) + (y & y)) ^ ((div(y, y)) * x)) & (((y - y) + (mod(x, x))) ^ ((x - x) + y));
	});
	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return ((div(x, x)) + (x-x)) - ((y | y) + (ucos(x)));
	});
	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return ((div(x, y)) | (y | y)) ^ ((x * y) * int8u(sqrt(y)));
	});
	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return usin((y * (usin(x))));
	});
	makeImage(++i, [] (int8u x, int8u y) -> int8u {
		return (x ^ y) ^ (y - x);
	});
}