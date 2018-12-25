#ifndef TWG_POINT_INCLUDED
#define TWG_POINT_INCLUDED

#include <math.h>
#include "twg/basics.h"

namespace twg
{

	//-------------------------------------------------------------------------
	template<class T, class T1> 
	class PointBase
	{
	public:
		typedef T 					ValueType;
		typedef T1					ComputeType;
		typedef const ComputeType	ComputeConst;
		typedef PointBase<T, T1> 	SelfType;
		typedef const SelfType		SelfConst;

		//---------------------------------------------------------------------
		T x, y;
		PointBase(T x = 0, T y = 0) : x(x), y(y) {}
		template<class T3>
		PointBase(const T3& t) : x(T(t.x)), y(T(t.y)) {}

		//---------------------------------------------------------------------
		ComputeConst getLength(void) const;
		ComputeConst getAngle(void) const;

		void toBasis(SelfConst& x1, SelfConst& y1);		
		void fromBasis(SelfConst& x1, SelfConst& y1);

		SelfConst rotate(ComputeConst& angle, 
						 SelfConst& center = SelfType());

		ComputeConst computeAngle(SelfConst& a) const;
		
		bool inRectangle(SelfConst& p1, SelfConst& p2) const;
		bool inTriangle(SelfConst& p1, 
						SelfConst& p2, 
						SelfConst& p3) const;

		//---------------------------------------------------------------------
		template<class N, class N1>
		operator PointBase<N, N1>();

		SelfType operator-(void);

		SelfType operator+=(SelfConst& a);
		SelfType operator-=(SelfConst& a);
		SelfType operator*=(ComputeConst &a);
		SelfType operator/=(ComputeConst &a);

		bool operator==(SelfConst& a);
		bool operator!=(SelfConst& a);
	};

	//-------------------------------------------------------------------------
	template<class PointType> 
	class PointIterator
	{
	public:
		typedef PointIterator SelfType;

		PointIterator(PointType a, PointType b);

		void assign(PointType a, PointType b);
		void onStart();
		bool isEnd();

		SelfType operator++();
		SelfType operator++(int);
		PointType operator*();
	private:
		PointType a, b, current;
	};

	//-------------------------------------------------------------------------
	typedef PointBase<int32, double>	Point_i;
	typedef PointBase<int32u, double>	Point_iu;
	typedef PointBase<int32&, double>	Point_ri;
	typedef PointBase<double, double>	Point_d;

//=============================================================================
//=============================================================================
//=============================================================================

//-----------------------------------------------------------------------------
template<class T, class T1> 
inline typename PointBase<T, T1>::ComputeConst PointBase<T, T1>::getLength(void) const {
	return sqrt(x*x + y*y);
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
inline typename PointBase<T, T1>::ComputeConst PointBase<T, T1>::getAngle(void) const {
	return atan2(y, x);
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
void PointBase<T, T1>::toBasis(SelfConst& x1, SelfConst& y1) {
	ValueType nx = (y*y1.x-x*y1.y)/(x1.y*y1.x-x1.x*y1.y);
	ValueType ny = -(x1.x*y-x*x1.y)/(x1.y*y1.x-x1.x*y1.y);
	x = nx;
	y = ny;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
void PointBase<T, T1>::fromBasis(SelfConst& x1, SelfConst& y1) {
	SelfType a = x1*x + y1*y;
	x = a.x;
	y = a.y;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
const PointBase<T, T1> PointBase<T, T1>::rotate(ComputeConst& angle, 
					   		  SelfConst& center) {
	T x1 = x - center.x;
	T y1 = y - center.y;
	x = cos(angle)*x1 - sin(angle)*y1 + center.x;
	y = sin(angle)*x1 + cos(angle)*y1 + center.y;
	
	return *this;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
typename PointBase<T, T1>::ComputeConst PointBase<T, T1>::computeAngle(SelfConst& a) const {
	ComputeConst angle = getAngle() - a.getAngle();
	angle = (angle < 0) ? -angle : angle;
	while (angle > 2*pi) angle -= 2*pi;
	return angle;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
inline bool PointBase<T, T1>::inRectangle(SelfConst& p1, SelfConst& p2) const {
	SelfConst p3(TWG_min(p1.x, p2.x), TWG_min(p1.y, p2.y));
	SelfConst p4(TWG_max(p1.x, p2.x), TWG_max(p1.y, p2.y));
	return (x <= p4.x && x >= p3.x && y <= p4.y && y >= p3.y);
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
inline bool PointBase<T, T1>::inTriangle(SelfConst& p1, 
						   SelfConst& p2, 
						   SelfConst& p3) const {
	return false;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
template<class N, class N1>
inline PointBase<T, T1>::operator PointBase<N, N1>() {
	PointBase<N, N1> a;
	a.x = x;
	a.y = y;
	return a;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
inline typename PointBase<T, T1>::SelfType PointBase<T, T1>::operator-(void) {
	return PointBase(-x, -y);
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
inline typename PointBase<T, T1>::SelfType PointBase<T, T1>::operator+=(SelfConst& a) {
	x += a.x;
	y += a.y;
	return *this;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
inline typename PointBase<T, T1>::SelfType PointBase<T, T1>::operator-=(SelfConst& a) {
	x -= a.x;
	y -= a.y;
	return *this;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
inline typename PointBase<T, T1>::SelfType PointBase<T, T1>::operator*=(ComputeConst &a) {
	x *= a;
	y *= a;
	return *this;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
inline typename PointBase<T, T1>::SelfType PointBase<T, T1>::operator/=(ComputeConst &a) {
	x /= a;
	y /= a;
	return *this;
}

//-----------------------------------------------------------------------------
template<class T, class T1, class T2, class T3> 
inline PointBase<T, T1> operator+(const PointBase<T, T1>& a, const PointBase<T2, T3> b) {
	return PointBase<T, T1>(a.x + T(b.x), a.y + T(b.y));
}

//-----------------------------------------------------------------------------
template<class T, class T1, class T2, class T3> 
inline PointBase<T, T1> operator-(const PointBase<T, T1>& a, const PointBase<T2, T3> b) {
	return PointBase<T, T1>(a.x - T(b.x), a.y - T(b.y));
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
inline PointBase<T, T1> operator+(const PointBase<T, T1>& a, const PointBase<T, T1> b) {
	return PointBase<T, T1>(a.x + b.x, a.y + b.y);
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
inline PointBase<T, T1> operator-(const PointBase<T, T1>& a, const PointBase<T, T1> b) {
	return PointBase<T, T1>(a.x - b.x, a.y - b.y);
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
inline PointBase<T, T1> operator*(const PointBase<T, T1>& a, const PointBase<T, T1> b) {
	return PointBase<T, T1>(a.x*b.x, a.y*b.y);
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
inline PointBase<T, T1> operator*(const PointBase<T, T1>& a, const T1 b) {
	return PointBase<T, T1>(a.x*b, a.y*b);
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
inline PointBase<T, T1> operator/(const PointBase<T, T1>& a, const T1 b) {
	return PointBase<T, T1>(a.x/b, a.y/b);
}

//-----------------------------------------------------------------------------
template<class T, class T1, class T3> 
inline PointBase<T, T1> operator*(const PointBase<T, T1>& a, const T3 b) {
	return PointBase<T, T1>(a.x*T1(b), a.y*T1(b));
}

//-----------------------------------------------------------------------------
template<class T, class T1, class T3> 
inline PointBase<T, T1> operator/(const PointBase<T, T1>& a, const T3 b) {
	return PointBase<T, T1>(a.x/T1(b), a.y/T1(b));
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
inline PointBase<T, T1> operator*(const T1 b, const PointBase<T, T1>& a) {
	return PointBase<T, T1>(a.x*b, a.y*b);
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
inline PointBase<T, T1> operator/(const T1 b, const PointBase<T, T1>& a) {
	return PointBase<T, T1>(a.x/b, a.y/b);
}

//-----------------------------------------------------------------------------
template<class T, class T1, class T3> 
inline PointBase<T, T1> operator*(const T3 b, const PointBase<T, T1>& a) {
	return PointBase<T, T1>(a.x*T1(b), a.y*T1(b));
}

//-----------------------------------------------------------------------------
template<class T, class T1, class T3> 
inline PointBase<T, T1> operator/(const T3 b, const PointBase<T, T1>& a) {
	return PointBase<T, T1>(a.x/T1(b), a.y/T1(b));
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
inline bool PointBase<T, T1>::operator==(SelfConst& a) {
	return x == a.x && y == a.y;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
inline bool PointBase<T, T1>::operator!=(SelfConst& a) {
	return x != a.x && y != a.y;
}

//-----------------------------------------------------------------------------
template<class PointType> 
inline PointIterator<PointType>::PointIterator(PointType a1, PointType b1) {
	assign(a1, b1);
}

//-----------------------------------------------------------------------------
template<class PointType> 
inline void PointIterator<PointType>::assign(PointType a1, PointType b1) {
	a.x = TWG_min(a1.x, b1.x);
	a.y = TWG_min(a1.y, b1.y);
	b.x = TWG_max(a1.x, b1.x);
	b.y = TWG_max(a1.y, b1.y);
	current = a;
}

//-----------------------------------------------------------------------------
template<class PointType> 
inline PointIterator<PointType> PointIterator<PointType>::operator++() {
	current.x++;
	if (current.x > b.x) {
		current.x = a.x;
		if (current.y <= b.y)
			current.y++;
	}
	return *this;
}

//-----------------------------------------------------------------------------
template<class PointType> 
inline PointIterator<PointType> PointIterator<PointType>::operator++(int) {
	return operator++();
}

//-----------------------------------------------------------------------------
template<class PointType> 
inline PointType PointIterator<PointType>::operator*() {
	return current;
}

//-----------------------------------------------------------------------------
template<class PointType> 
inline void PointIterator<PointType>::onStart() {
	current = a;
}

//-----------------------------------------------------------------------------
template<class PointType> 
inline bool PointIterator<PointType>::isEnd() {
	return current.x == a.x && current.y-1 == b.y;
}

}

#endif // TWG_POINT_INCLUDED