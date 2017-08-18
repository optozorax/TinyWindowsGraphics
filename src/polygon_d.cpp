#include "twg/image/image_drawing.h"

namespace twg
{

//-----------------------------------------------------------------------------
Polygon_d& Polygon_d::move(Point_d diff) {
	for (i : array)
		i += diff;
}

//-----------------------------------------------------------------------------
Polygon_d& Polygon_d::rotate(double angle, Point_d center) {
	for (i : array)
		i.rotate(angle, center);
}

//-----------------------------------------------------------------------------
Polygon_d& Polygon_d::scale(Point_d scale) {
	for (i: array) {
		i.x *= scale.x;
		i.y *= scale.y;
	}
}

//-----------------------------------------------------------------------------
Polygon_d& Polygon_d::toBasis(Point_d newOX, Point_d newOY) {
	for (i : array)
		i.toBasis(newOX, newOY);
}

//-----------------------------------------------------------------------------
Polygon_d& Polygon_d::fromBasis(Point_d oldOX, Point_d oldOY) {
	for (i : array)
		i.fromBasis(oldOX, oldOY);
}

//-----------------------------------------------------------------------------
Polygon_d& Polygon_d::flipAxis(Point_d axis) {
	for (i : array) {
		double t = (axis.x*i.x + axis.y*i.y)/(axis.x*axis.x + axis.y*axis.y);
		i = axis*2.0*t - i;
	}
}

//-----------------------------------------------------------------------------
Polygon_d computeEllipse(Point_d radius) {
	Polygon_d poly;
	double maxr = TWG_max(radius.x, radius.y);
	int32u count = pi*maxr*maxr + 30;
	for (int i = 0; i < count; ++i) {
		double angle = 2.0*pi * double(i) / count;
		double x = cos(angle);
		double y = sin(angle);
		poly.array.push_back(Point_d(x, y));
	}
	poly.scale(radius);
	return poly;
}

//-----------------------------------------------------------------------------
Polygon_d computeArc(double radius, 
					 double startAngle,
					 double endAngle) {

}

//-----------------------------------------------------------------------------
Polygon_d computePie(double radius, 
					 double startAngle,
					 double endAngle) {
	Polygon_d arc = computeArc(radius, startAngle, endAngle);
	arc.array.insert(arc.array.begin(), Point_d(0, 0));
	arc.array.push_back(Point_d(0, 0));
	return arc;
}

//-----------------------------------------------------------------------------
Polygon_d computeRoundRect(Point_d size,
						   double rLU,
						   double rRU,
						   double rRD,
						   double rLD) {

}

//-----------------------------------------------------------------------------
Polygon_d computeRegularPolygon(int32u n, double radius) {
	std::vector<Point_d> poly;
	for (int i = 0; i < n; ++i)
		poly.push_back(Point_d(cos(2.0*pi*i/n),sin(2.0*pi*i/n)) * radius);
	return Polygon_d(poly);
}

}