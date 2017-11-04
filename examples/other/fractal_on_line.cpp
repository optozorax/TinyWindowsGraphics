#include <fstream>
#include <twg/image.h>
#include <twg/image/image_drawing.h>

using namespace twg;

template<class Draw>
void drawFractal(ImageDrawing_aa& img, 
				 Polygon_d& poly, const 
				 Draw& draw, 
				 int8u maxDepth = 30, 
				 double minRadius = 0.5, 
				 int32u depth = 0) {
	if (depth < maxDepth && (poly.array[1]-poly.array[0]).getLength() > minRadius) {
		draw(poly, depth);
		for (int8u i = 0; i < poly.array.size()-1; ++i) {
			Polygon_d poly1 = poly;
			poly1.move(-poly1.array[0]);

			Point_d ox = poly.array[poly.array.size()-1]-poly.array[0];
			ox /= ox.getLength();
			Point_d oy = ox;
			oy.rotate(pi/2.0);

			poly1.toBasis(ox, oy);

			Point_d nx = poly.array[i+1] - poly.array[i];
			nx /= (poly.array[poly.array.size()-1]-poly.array[0]).getLength();
			Point_d ny = nx;
			ny.rotate(pi/2.0);
			poly1.fromBasis(nx, ny);
			poly1.move(poly.array[i]);

			drawFractal(img, poly1, draw, maxDepth, minRadius, depth+1);
		}
	}
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
//int main() {
	using namespace std;
	int n;
	Polygon_d poly;
	Point_i size;

	ifstream fin("line.txt");
	if (fin.is_open()) {
		fin >> size.x >> size.y;
		fin >> n;
		Point_i x;
		for (int i = 0; i < n; ++i) {
			fin >> x.x >> x.y;
			poly.array.push_back(x);
		}
		fin.close();
	} else {
		poly.array.push_back(Point_d(-1, 0));
		poly.array.push_back(Point_d(0, 1));
		poly.array.push_back(Point_d(1, 0));
		//poly.array.push_back(Point_d(2, 0));
		poly.scale(Point_d(400, 400));
		poly.move(Point_d(1920/2, 230));

		n = 3;
		size = Point_i(1920, 1080);
	}

	ImageDrawing_aa img(size);
	img.clear(Black);

	double length = (poly.array[1]-poly.array[0]).getLength();

	drawFractal(img, poly, [&] (Polygon_d& poly, int8u depth) {
		double length2 = (poly.array[1]-poly.array[0]).getLength();
		Color clr = getColorBetween(computeCycle(sqrt(length2/length), CYCLE_SIN), Yellow, Miku);
		img.setBrush(Brush(setAlpha(clr, 50)));
		img.drawPolygon(poly);
	});

	saveToBmp(&img, L"fractal_on_line.bmp");
}