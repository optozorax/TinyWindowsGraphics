#ifndef TWG_BASICS_INCLUDED
#define TWG_BASICS_INCLUDED

#include <windef.h>
#include <stdint.h>
#include <string>

namespace twg
{

	typedef int8_t	int8;
	typedef int16_t	int16;
	typedef int32_t	int32; 
	typedef int64_t	int64;
	
	typedef uint8_t		int8u;
	typedef uint16_t	int16u;
	typedef uint32_t	int32u;
	typedef uint64_t	int64u;

	enum CursorType : int32u;
	enum CycleType : int32u;
	enum MessageType : int32u;
	enum MessageReturnType : int32u;

	//-------------------------------------------------------------------------
	const double pi = 3.14159265358979323846;

	double 	deg2rad(double deg);
	double 	rad2deg(double rad);
	double 	computeCycle(double pos, CycleType type);

	void 	sleep(int32u);
	bool 	isKeyDown(int16u);
	int32u 	timeGet(void);
	void 	setCursor(CursorType type);

	MessageReturnType messageBox(std::string text, 
								 std::string caption,
								 MessageType type);

	//-------------------------------------------------------------------------
	enum CursorType : int32u
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

	enum MessageType : int32u
	{
		// https://msdn.microsoft.com/ru-ru/library/windows/desktop/ms645505(v=vs.85).aspx
	};

	enum MessageReturnType : int32u
	{
		// https://msdn.microsoft.com/ru-ru/library/windows/desktop/ms645505(v=vs.85).aspx
	};

	enum CycleType : int32u
	{
		CYCLE_NULL, // Обрезается на нуле и на единице: /
		CYCLE_SIN, // y=x на промежутке [0, 1] повторяется как sin: /\/\
		CYCLE_TAN // y=x на промежутке [0, 1] повторяется как tg: / / /
	};

//=============================================================================
//=============================================================================
//=============================================================================

//-----------------------------------------------------------------------------
inline double deg2rad(double deg) {
	return deg * pi / 180.0;
}

//-----------------------------------------------------------------------------
inline double rad2deg(double rad) {
	return rad * 180.0 / pi;
}

//-----------------------------------------------------------------------------
inline void sleep(int32u) {

}

//-----------------------------------------------------------------------------
inline bool isKeyDown(int16u) {

}

//-----------------------------------------------------------------------------
inline int32u timeGet(void) {
	return 0;
}

}

#endif // TWG_BASICS_INCLUDED