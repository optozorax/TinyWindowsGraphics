#include <windows.h>
#include "twg/basics.h"

namespace twg 
{

//-----------------------------------------------------------------------------
void sleep(int32u time) {
	Sleep(time);
}

//-----------------------------------------------------------------------------
bool isKeyDown(int16u) {

}

//-----------------------------------------------------------------------------
int32u timeGet(void) {
	return GetTickCount();
}

//-----------------------------------------------------------------------------
double computeCycle(double pos, CycleType type) {

}

//-----------------------------------------------------------------------------
void setCursor(CursorType type) {

}

//-----------------------------------------------------------------------------
MessageReturnType messageBox(std::string text, 
							 std::string caption,
							 MessageType type) {
		
}

}