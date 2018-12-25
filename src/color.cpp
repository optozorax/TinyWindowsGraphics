#include <color/color.hpp>
#include <twg/color.h>
#include <math.h>

namespace twg 
{

//-----------------------------------------------------------------------------
Color hsl(const int8u h,
		  const int8u s,
		  const int8u l) {
	color::hsl<int8u> clr1({h, s, l});
	color::rgb<int8u> clr2;
	clr2 = clr1;
	return rgb(clr2.get(0), clr2.get(1), clr2.get(2));
}

//-----------------------------------------------------------------------------
Color hsla(const int8u h,
		   const int8u s,
		   const int8u l,
		   const int8u a) {
	return setAlpha(hsl(h, s, l), a);
}

//-----------------------------------------------------------------------------
int8u setHue(Color clr, int8u h) {
	color::rgb<int8u> clr1({getRed(clr), getGreen(clr), getBlue(clr)});
	color::hsl<int8u> clr2;
	clr2 = clr1;
	clr2.set(0, h);
	clr1 = clr2;
	return rgb(clr1.get(0), clr1.get(1), clr1.get(2));
}

//-----------------------------------------------------------------------------
int8u setSaturation(Color clr, int8u s) {
	color::rgb<int8u> clr1({getRed(clr), getGreen(clr), getBlue(clr)});
	color::hsl<int8u> clr2;
	clr2 = clr1;
	clr2.set(1, s);
	clr1 = clr2;
	return rgb(clr1.get(0), clr1.get(1), clr1.get(2));
}

//-----------------------------------------------------------------------------
int8u setLightness(Color clr, int8u l) {
	color::rgb<int8u> clr1({getRed(clr), getGreen(clr), getBlue(clr)});
	color::hsl<int8u> clr2;
	clr2 = clr1;
	clr2.set(2, l);
	clr1 = clr2;
	return rgb(clr1.get(0), clr1.get(1), clr1.get(2));
}

//-----------------------------------------------------------------------------
int8u getHue(Color clr) {
	color::rgb<int8u> clr1({getRed(clr), getGreen(clr), getBlue(clr)});
	color::hsl<int8u> clr2;
	clr2 = clr1;
	return clr2.get(0);
}

//-----------------------------------------------------------------------------
int8u getSaturation(Color clr) {
	color::rgb<int8u> clr1({getRed(clr), getGreen(clr), getBlue(clr)});
	color::hsl<int8u> clr2;
	clr2 = clr1;
	return clr2.get(1);
}

//-----------------------------------------------------------------------------
int8u getLightness(Color clr) {
	color::rgb<int8u> clr1({getRed(clr), getGreen(clr), getBlue(clr)});
	color::hsl<int8u> clr2;
	clr2 = clr1;
	return clr2.get(2);
}

//-----------------------------------------------------------------------------
Color getColorBetween(double pos, Color clr1, Color clr2) {
	computeCycle(pos, CYCLE_NULL);
	auto interpolate = [] (int8u a, int8u b, double pos) -> int8u {
		double ad = a/255.0;
		double bd = b/255.0;
		ad *= ad;
		bd *= bd;
		return sqrt(ad*(1.0-pos) + bd*pos) * 255.0;
	};
	return rgba(interpolate(getRed(clr1), getRed(clr2), pos),
			    interpolate(getGreen(clr1), getGreen(clr2), pos),
			    interpolate(getBlue(clr1), getBlue(clr2), pos),
			    getAlpha(clr1)*(1.0-pos) + getAlpha(clr2)*pos);
}

//-----------------------------------------------------------------------------
Color getGrayHue(double hue) {
	return getColorBetween(hue, White, Black);
}

//-----------------------------------------------------------------------------
Color getRainbow(double pos) {
	std::vector<Color> clrs;
	clrs.push_back(Red);
	clrs.push_back(Fuchsia);
	clrs.push_back(Blue);
	clrs.push_back(Aqua);
	clrs.push_back(Green);
	clrs.push_back(Yellow);
	clrs.push_back(Red);

	return getGradient(pos, clrs);
}

//-----------------------------------------------------------------------------
Color getGradient(double pos, std::vector<Color> colors) {
	std::vector<double> sizes(colors.size()-1, 1);
	return getGradient(pos, colors, sizes);
}

//-----------------------------------------------------------------------------
Color getGradient(double pos, 
				  std::vector<Color> colors, 
				  std::vector<double> sizes) {
	computeCycle(pos, CYCLE_NULL);
	double sum = 0;
	for (int i = 0; i < sizes.size(); ++i) sum += sizes[i];
	for (int i = 0; i < sizes.size(); ++i) sizes[i] /= sum;
	for (int i = 0; i < sizes.size(); ++i) {
		if (pos <= sizes[i])
			return getColorBetween(pos/sizes[i], colors[i], colors[i+1]);
		pos -= sizes[i];
	}
}

}