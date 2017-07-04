#ifndef TWG_BASICS_INCLUDED
#define TWG_BASICS_INCLUDED

#include <windef.h>

namespace twg
{

	typedef signed		__int8	int8;
	typedef signed		__int16	int16;
	typedef signed		__int32	int32; 
	typedef signed		__int64	int64;
	
	typedef unsigned	__int8	int8u; 
	typedef unsigned	__int16	int16u;
	typedef unsigned	__int32	int32u;
	typedef unsigned	__int64	int64u;

	enum CursorType;
	enum CycleType;

	//-------------------------------------------------------------------------
	const double pi = 3.14159265358979323846;

	double deg2rad(double deg);
	double rad2deg(double rad);

	void 	sleep(int32u);
	bool 	isKeyDown(int16u);
	int32u 	timeGet(void);

	void setCursor(CursorType type);

	double computeCycle(double pos, CycleType type);

	//-------------------------------------------------------------------------
	enum CursorType
	{
		CURSOR_NULL,
		CURSOR_STANDART,
		CURSOR_CLICK,
		CURSOR_WAIT,
		CURSOR_CROSS,
		CURSOR_HELP,
		CURSOR_STANDART_HELP,
		CURSOR_FORBIDDEN,
		CURSOR_MOVE,
		CURSOR_UP,

		CURSOR_SIZE_UP_DOWN,
		CURSOR_SIZE_LEFT_RIGHT,
		CURSOR_SIZE_LEFT_DOWN_CORNER,
		CURSOR_SIZE_RIGHT_UP_CORNER
	};

	enum CycleType {
		CYCLE_NULL,
		CYCLE_SIN,
		CYCLE_TAN
	};
}

//=============================================================================
//=============================================================================
//=============================================================================

//-----------------------------------------------------------------------------
inline double twg::deg2rad(double deg)
{
	return deg * pi / 180.0;
}

//-----------------------------------------------------------------------------
inline double twg::rad2deg(double rad)
{
	return rad * 180.0 / pi;
}

#endif // TWG_BASICS_INCLUDED