#ifndef TWG_POINT_INCLUDED
#define TWG_POINT_INCLUDED

#include <math.h>
#include "twg/basics.h"

namespace twg
{

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

		//---------------------------------------------------------------------
		ComputeType getLength(void);
		ComputeType getAngle(void);

		void toBasis(SelfConst& x1, SelfConst& y1);		
		void fromBasis(SelfConst& x1, SelfConst& y1);

		void rotate(ComputeConst& angle, 
					SelfConst& center = SelfType());

		ComputeType computeAngle(SelfConst& a);
		
		bool inRectangle(SelfConst& p1, SelfConst& p2);
		bool inTriangle(SelfConst& p1, 
						SelfConst& p2, 
						SelfConst& p3);

		//---------------------------------------------------------------------
		template<class N, class N1>
		operator PointBase<N, N1>();

		SelfType operator-(void);

		SelfType operator+=(SelfConst& a);
		SelfType operator-=(SelfConst& a);
		SelfType operator*=(ComputeType &a);
		SelfType operator/=(ComputeType &a);

		SelfType operator+(SelfConst& a);
		SelfType operator-(SelfConst& a);
		SelfType operator*(ComputeType &a);
		SelfType operator/(ComputeType &a);

		bool operator==(SelfConst& a);
		bool operator!=(SelfConst& a);
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
typename PointBase<T, T1>::ComputeType PointBase<T, T1>::getLength(void) {
	return sqrt(x*x + y*y);
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
typename PointBase<T, T1>::ComputeType PointBase<T, T1>::getAngle(void) {
	return atan2(y, x);
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
void PointBase<T, T1>::toBasis(SelfConst& x1, SelfConst& y1) {

}

//-----------------------------------------------------------------------------
template<class T, class T1> 
void PointBase<T, T1>::fromBasis(SelfConst& x1, SelfConst& y1) {

}

//-----------------------------------------------------------------------------
template<class T, class T1> 
void PointBase<T, T1>::rotate(ComputeConst& angle, 
					   		  SelfConst& center) {
	T x1 = x - center.x;
	T y1 = y - center.y;
	x = cos(angle)*x1 - sin(angle)*y1 + center.x;
	y = sin(angle)*x1 + cos(angle)*y1 + center.y;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
typename PointBase<T, T1>::ComputeType PointBase<T, T1>::computeAngle(SelfConst& a) {
	ComputeType angle = getAngle() - a.getAngle();
	angle = (angle < 0) ? -angle : angle;
	return angle;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
bool PointBase<T, T1>::inRectangle(SelfConst& p1, SelfConst& p2) {
	return false;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
bool PointBase<T, T1>::inTriangle(SelfConst& p1, 
						   SelfConst& p2, 
						   SelfConst& p3) {
	return false;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
template<class N, class N1>
PointBase<T, T1>::operator PointBase<N, N1>() {
	PointBase<N, N1> a;
	a.x = x;
	a.y = y;
	return a;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
typename PointBase<T, T1>::SelfType PointBase<T, T1>::operator-(void) {
	return PointBase(-x, -y);
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
typename PointBase<T, T1>::SelfType PointBase<T, T1>::operator+=(SelfConst& a) {
	x += a.x;
	y += a.y;
	return *this;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
typename PointBase<T, T1>::SelfType PointBase<T, T1>::operator-=(SelfConst& a) {
	x -= a.x;
	y -= a.y;
	return *this;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
typename PointBase<T, T1>::SelfType PointBase<T, T1>::operator*=(ComputeType &a) {
	x *= a;
	y *= a;
	return *this;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
typename PointBase<T, T1>::SelfType PointBase<T, T1>::operator/=(ComputeType &a) {
	x /= a;
	y /= a;
	return *this;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
typename PointBase<T, T1>::SelfType PointBase<T, T1>::operator+(SelfConst& a) {
	return PointBase(x+a.x, y+a.y);
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
typename PointBase<T, T1>::SelfType PointBase<T, T1>::operator-(SelfConst& a) {
	return PointBase(x-a.x, y-a.y);
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
typename PointBase<T, T1>::SelfType PointBase<T, T1>::operator*(ComputeType &a) {
	return PointBase(x*a, y*a);
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
typename PointBase<T, T1>::SelfType PointBase<T, T1>::operator/(ComputeType &a) {
	return PointBase(x/a, y/a);
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
bool PointBase<T, T1>::operator==(SelfConst& a) {
	return x == a.x && y == a.y;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
bool PointBase<T, T1>::operator!=(SelfConst& a) {
	return x != a.x && y != a.y;
}

}

#endif // TWG_POINT_INCLUDED