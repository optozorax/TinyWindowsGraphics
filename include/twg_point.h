#ifndef TWG_POINT_INCLUDED
#define TWG_POINT_INCLUDED

namespace twg
{

	template<class T, class T1> class PointBase
	{
	public:
		typedef T 					ValueType;
		typedef T1					ComputeType;
		typedef PointBase<T, T1> 	SelfType;
		typedef const SelfType		SelfConst;

		//---------------------------------------------------------------------
		T x, y;
		PointBase(T x_ = 0, T y_ = 0);

		//---------------------------------------------------------------------
		ComputeType getLength(void);
		ComputeType getAngle(void);

		void toBasis(SelfConst x1, SelfConst y1);
		void fromBasis(SelfConst x1, SelfConst y1);

		void rotate(const ComputeType angle, 
					const SelfType &center = SelfType());

		ComputeType computeAngle(SelfType a);
		
		bool inRectanglenui(const SelfType p1, const SelfType p2);
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

	typedef PointBase<int32, double>	Point_i;
	typedef PointBase<double>			Point_d;

}

//=============================================================================
//=============================================================================
//=============================================================================

#endif // TWG_POINT_INCLUDED