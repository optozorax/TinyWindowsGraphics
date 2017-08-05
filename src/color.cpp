#include <twg/color.h>

namespace twg 
{

//-----------------------------------------------------------------------------
Color getColorBetween(double pos,
					  Color clr1, 
					  Color clr2,
					  CycleType cycle = CYCLE_NULL) {

}

//-----------------------------------------------------------------------------
Color getGrayHue(double hue, CycleType cycle = CYCLE_NULL) {

}

//-----------------------------------------------------------------------------
Color getRainbow(double pos, CycleType cycle = CYCLE_NULL) {

}

//-----------------------------------------------------------------------------
Color getGradient(double pos, 
				  std::vector<Color> colors,
				  CycleType cycle = CYCLE_NULL) {

}

//-----------------------------------------------------------------------------
Color getGradient(double pos, 
				  std::vector<Color>& colors, 
				  std::vector<double>& sizes,
				  CycleType cycle = CYCLE_NULL) {

}

//-----------------------------------------------------------------------------
Color computeOverlay(Color upper, Color lower) {

}

}