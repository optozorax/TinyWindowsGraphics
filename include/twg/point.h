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
		typedef PointBase<T, T1> 	SelfType;
		typedef const SelfType		SelfConst;

		//---------------------------------------------------------------------
		T x, y;
		PointBase(T x = 0, T y = 0) : x(x), y(y) {}

		//---------------------------------------------------------------------
		ComputeType getLength(void);
		ComputeType getAngle(void);

		void toBasis(SelfConst x1, SelfConst y1);		
		void fromBasis(SelfConst x1, SelfConst y1);

		void rotate(const ComputeType angle, 
					const SelfType &center = SelfType());

		ComputeType computeAngle(SelfType a);
		
		bool inRectangle(const SelfType p1, const SelfType p2);
		bool inTriangle(const SelfType p1, 
						const SelfType p2, 
						const SelfType p3);

		//---------------------------------------------------------------------
		template<class N, class N1>
		operator PointBase<N, N1>();

		SelfType operator-(void);

		SelfType operator+=(SelfType &a);
		SelfType operator-=(SelfType &a);
		SelfType operator*=(ComputeType &a);
		SelfType operator/=(ComputeType &a);

		SelfType operator+(SelfType &a);
		SelfType operator-(SelfType &a);
		SelfType operator*(ComputeType &a);
		SelfType operator/(ComputeType &a);

		bool operator==(SelfType &a);
		bool operator!=(SelfType &a);
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
void PointBase<T, T1>::toBasis(SelfConst x1, SelfConst y1) {

}

//-----------------------------------------------------------------------------
template<class T, class T1> 
void PointBase<T, T1>::fromBasis(SelfConst x1, SelfConst y1) {

}

//-----------------------------------------------------------------------------
template<class T, class T1> 
void PointBase<T, T1>::rotate(const ComputeType angle, 
					   const SelfType &center = SelfType()) {

}

//-----------------------------------------------------------------------------
template<class T, class T1> 
typename PointBase<T, T1>::ComputeType PointBase<T, T1>::computeAngle(SelfType a) {
	ComputeType angle = getAngle() - a.getAngle();
	angle = (angle < 0) ? -angle : angle;
	return angle;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
bool PointBase<T, T1>::inRectangle(const SelfType p1, const SelfType p2) {
	return false;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
bool PointBase<T, T1>::inTriangle(const SelfType p1, 
						   const SelfType p2, 
						   const SelfType p3) {
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
	return *this;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
typename PointBase<T, T1>::SelfType PointBase<T, T1>::operator+=(SelfType &a) {
	return *this;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
typename PointBase<T, T1>::SelfType PointBase<T, T1>::operator-=(SelfType &a) {
	return *this;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
typename PointBase<T, T1>::SelfType PointBase<T, T1>::operator*=(ComputeType &a) {
	return *this;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
typename PointBase<T, T1>::SelfType PointBase<T, T1>::operator/=(ComputeType &a) {
	return *this;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
typename PointBase<T, T1>::SelfType PointBase<T, T1>::operator+(SelfType &a) {
	return *this;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
typename PointBase<T, T1>::SelfType PointBase<T, T1>::operator-(SelfType &a) {
	return *this;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
typename PointBase<T, T1>::SelfType PointBase<T, T1>::operator*(ComputeType &a) {
	return *this;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
typename PointBase<T, T1>::SelfType PointBase<T, T1>::operator/(ComputeType &a) {
	return *this;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
bool PointBase<T, T1>::operator==(SelfType &a) {
	return *this;
}

//-----------------------------------------------------------------------------
template<class T, class T1> 
bool PointBase<T, T1>::operator!=(SelfType &a) {
	return *this;
}

}

#endif // TWG_POINT_INCLUDED