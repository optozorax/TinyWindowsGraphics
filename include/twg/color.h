#ifndef TWG_COLOR_INCLUDED
#define TWG_COLOR_INCLUDED

#include <vector>

#include "twg/basics.h"

namespace twg
{
	union ColorUnion;

	/** @defgroup Цвета
	
		Написать что используются полупрозрачные цвета и как они работают.

		@{
	 */

	/** Цвет пикселя в изображении Windows. Содержит 4 байта. Каждый байт - оттенок красного, зеленого, или синего. Порядок расположения байтов: blue, green, red, alpha. */
	typedef int32u Color_order_bgra_32;
	typedef Color_order_bgra_32 Color;

	//-------------------------------------------------------------------------
	/** Возвращает цвет по параметрам красного, зеленого и синего. Альфа-канал всегда равен 255. */
	Color rgb(const int8u r, 
			  const int8u g, 
			  const int8u b);
	/** Возвращает цвет по параметрам красного, зеленого, синего и альфа-канала. */
	Color rgba(const int8u r, 
			   const int8u g, 
			   const int8u b, 
			   const int8u a);
	/** Возвращает цвет по параметрам оттенка, контраста и яркости. Альфа-канал всегда равен 255. */
	Color hsl(const int8u h, /// Оттенок
			  const int8u s, /// Контраст
			  const int8u l); /// Яркость
	/** Возвращает цвет по параметрам оттенка, контраста, яркости и альфа-канала. */
	Color hsla(const int8u h, /// Оттенок
			   const int8u s, /// Контраст
			   const int8u l, /// Яркость
			   const int8u a);

	/** Возвращает копию заданного цвета с другим альфа-каналом. */
	Color setAlpha(Color clr, int8u a);
	/** Возвращает копию заданного цвета с другим оттенком красного. */
	Color setRed(Color clr, int8u r);
	/** Возвращает копию заданного цвета с другим оттенком зеленого. */
	Color setGreen(Color clr, int8u g);
	/** Возвращает копию заданного цвета с другим оттенком синего. */
	Color setBlue(Color clr, int8u b);
	/** Возвращает копию заданного цвета с другим оттенком. */
	int8u setHue(Color clr, int8u h);
	/** Возвращает копию заданного цвета с другим контрастом. */
	int8u setSaturation(Color clr, int8u s);
	/** Возвращает копию заданного цвета с другой яркостью. */
	int8u setLightness(Color clr, int8u l);

	int8u getAlpha(Color clr);
	int8u getRed(Color clr);
	int8u getGreen(Color clr);
	int8u getBlue(Color clr);
	int8u getHue(Color clr);
	int8u getSaturation(Color clr);
	int8u getLightness(Color clr);

	/** Возвращает какой будет цвет на градиенте двух цветов clr1 и clr2 в позиции pos, которая может меняться от 0 до 1.
		@note При указании pos больше чем 1, возвращается цвет на 1. Если pos меньше 0, то возвращается цвет на 0.
		@note Градиент реализуется физически корректно.
	 */
	Color getColorBetween(double pos, Color clr1,  Color clr2);
	Color getGrayHue(double hue);
	/** Возвращает цвет из градиента радуги в положении pos. */
	Color getRainbow(double pos);
	Color getGradient(double pos, std::vector<Color> colors);
	Color getGradient(double pos, 
					  std::vector<Color> colors, 
					  std::vector<double> sizes);

	/** Рассчитывает наложение двух полупрозрачных пикселей. */
	Color computeOverlay(Color upper, Color lower);
	
	//-------------------------------------------------------------------------
	const Color Transparent 	= rgba(0, 0, 0, 0);

	const Color White 			= rgb(0xFF, 0xFF, 0xFF);
	const Color Black 			= rgb(0x00, 0x00, 0x00);

	const Color Red 			= rgb(0xFF, 0x00, 0x00);
	const Color Green 			= rgb(0x00, 0xFF, 0x00);
	const Color Blue 			= rgb(0x00, 0x00, 0xFF);

	const Color Yellow 			= rgb(0xFF, 0xFF, 0x00);
	const Color Fuchsia 		= rgb(0xFF, 0x00, 0xFF);
	const Color Aqua 			= rgb(0x00, 0xFF, 0xFF);

	const Color Maroon 			= rgb(0x80, 0x00, 0x00);
	const Color Oliv 			= rgb(0x80, 0x80, 0x00);
	const Color Purple 			= rgb(0x80, 0x00, 0x80);
	const Color Teal 			= rgb(0x00, 0x80, 0x80);
	const Color Navy 			= rgb(0x00, 0x00, 0x80);
	const Color Gray 			= rgb(0x80, 0x80, 0x80);

	const Color DarkBlue 		= rgb(0x4D, 0x4D, 0xFF);
	const Color LightBlue 		= rgb(0x00, 0xBF, 0xFF);
	const Color Orange 			= rgb(0xFF, 0x80, 0x00);
	const Color Gold 			= rgb(0xFF, 0xD7, 0x00);
	const Color Indigo 			= rgb(0x4B, 0x00, 0x80);
	const Color Silver			= rgb(0xC0, 0xC0, 0xC0);
	const Color Violet 			= rgb(0xEE, 0x80, 0xEE);

	const Color Ubuntu 			= rgb(0x77, 0x21, 0x6F);
	const Color UbuntuOrange 	= rgb(0xE9, 0x54, 0x20);
	const Color Bitcoin 		= rgb(0xFF,	0x99, 0x00);
	const Color Miku 			= rgb(0x09, 0xED, 0xC3);
	const Color Dollar 			= rgb(0x85, 0xbb, 0x65);
	const Color SeaGreen 		= rgb(0x52, 0x9B, 0x2F);
	const Color Pink 			= rgb(0xF9, 0x26, 0x72);
	const Color GreenYellow 	= rgb(0xAD, 0xFF, 0x2F);

	//-------------------------------------------------------------------------
	union ColorUnion {
		struct ColorStruct { 
			ColorStruct(const int8u r, const int8u g, const int8u b, const int8u a) : r(r), g(g), b(b), a(a) {}
			int8u b, g, r, a; 
		} rgba;
		Color color;

		ColorUnion(const Color& color) : color(color) {}
		ColorUnion(const int8u r, const int8u g, const int8u b, const int8u a) : rgba(r, g, b, a) {}
	};

	/** @} */

//=============================================================================
//=============================================================================
//=============================================================================

//-----------------------------------------------------------------------------
inline Color rgb(const int8u r, 
				 const int8u g, 
				 const int8u b) {
	return rgba(r, g, b, 255);
}

//-----------------------------------------------------------------------------
inline Color rgba(const int8u r, 
				  const int8u g, 
				  const int8u b, 
				  const int8u a) {
	return b * 0x1 |
		   g * 0x100 |
		   r * 0x10000 |
		   a * 0x1000000;
}

//-----------------------------------------------------------------------------
inline Color setAlpha(Color clr, int8u a) {
	return (clr & ~(getAlpha(clr) * 0x1000000)) | (a * 0x1000000);
}

//-----------------------------------------------------------------------------
inline Color setRed(Color clr, int8u r) {
	return (clr & ~(getRed(clr) * 0x10000)) | (r * 0x10000);
}

//-----------------------------------------------------------------------------
inline Color setGreen(Color clr, int8u g) {
	return (clr & ~(getGreen(clr) * 0x100)) | (g * 0x100);
}

//-----------------------------------------------------------------------------
inline Color setBlue(Color clr, int8u b) {
	return (clr & ~(getBlue(clr) * 0x1)) | (b * 0x1);
}

//-----------------------------------------------------------------------------
inline int8u getAlpha(Color clr) {
	return clr >> 24;
}

//-----------------------------------------------------------------------------
inline int8u getRed(Color clr) {
	return (clr << 8) >> 24;
}

//-----------------------------------------------------------------------------
inline int8u getGreen(Color clr) {
	return (clr << 16) >> 24;
}

//-----------------------------------------------------------------------------
inline int8u getBlue(Color clr) {
	return (clr << 24) >> 24;
}

//-----------------------------------------------------------------------------
inline Color computeOverlay(Color upper, Color lower) {
	ColorUnion up(upper);
	ColorUnion low(lower);

	int32u upr = up.rgba.r;
	int32u upg = up.rgba.g;
	int32u upb = up.rgba.b;
	int32u upa = up.rgba.a;

	int32u lowr = low.rgba.r;
	int32u lowg = low.rgba.g;
	int32u lowb = low.rgba.b;
	int32u lowa = low.rgba.a;

    return rgba((int8u)(((upr - lowr) * upa + (lowr << 8)) >> 8),
				(int8u)(((upg - lowg) * upa + (lowg << 8)) >> 8),
				(int8u)(((upb - lowb) * upa + (lowb << 8)) >> 8),
				(int8u)((upa + lowa) - ((lowa * upa + 255) >> 8)));
}
	
}

#endif // TWG_COLOR_INCLUDED