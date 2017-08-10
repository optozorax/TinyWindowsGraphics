#include <twg/color.h>

namespace twg 
{

//-----------------------------------------------------------------------------
Color getColorBetween(double pos, Color clr1, Color clr2) {
	computeCycle(pos, CYCLE_NULL);
	double k = 1-pos;
	return rgba(getRed(clr1)*k + getRed(clr2)*pos,
			    getGreen(clr1)*k + getGreen(clr2)*pos,
			    getBlue(clr1)*k + getBlue(clr2)*pos,
			    getAlpha(clr1)*k + getAlpha(clr2)*pos);
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