#include <string>
#include <windows.h>
#include <math.h>
#include "twg/basics.h"

namespace twg 
{

//-----------------------------------------------------------------------------
void sleep(int32u time) {
	Sleep(time);
}

//-----------------------------------------------------------------------------
bool isKeyDown(KeyType key) {
	return (-(GetKeyState(key) >> 8));
}

//-----------------------------------------------------------------------------
int32u timeGet(void) {
	return GetTickCount();
}

//-----------------------------------------------------------------------------
double computeCycle(double pos, CycleType type) {
	switch (type) {
		case CYCLE_NULL:
			return (pos < 0) ? 0 : ((pos > 1) ? 1 : pos);
		case CYCLE_SIN:
			pos = fmod(fabs(pos), 2.0);
			pos = (pos > 1) ? 2-pos : pos;
			return pos;
		case CYCLE_TAN:
			return fmod(fabs(pos), 1.0);
	}
}

//-----------------------------------------------------------------------------
void setCursor(CursorType type) {
	switch (type) {
		case CURSOR_NULL:
			SetCursor(NULL);
			break;
		case CURSOR_STANDART:
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			break;
		case CURSOR_CLICK:
			SetCursor(LoadCursor(NULL, IDC_HAND));
			break;
		case CURSOR_WAIT:
			SetCursor(LoadCursor(NULL, IDC_WAIT));
			break;
		case CURSOR_CROSS:
			SetCursor(LoadCursor(NULL, IDC_CROSS));
			break;
		case CURSOR_HELP:
			SetCursor(LoadCursor(NULL, IDC_HELP));
			break;
		case CURSOR_STANDART_WAIT:
			SetCursor(LoadCursor(NULL, IDC_APPSTARTING));
			break;
		case CURSOR_TEXT:
			SetCursor(LoadCursor(NULL, IDC_IBEAM));
			break;
		case CURSOR_FORBIDDEN:
			SetCursor(LoadCursor(NULL, IDC_NO));
			break;
		case CURSOR_MOVE:
			SetCursor(LoadCursor(NULL, IDC_SIZEALL));
			break;
		case CURSOR_UP:
			SetCursor(LoadCursor(NULL, IDC_UPARROW));
			break;
		case CURSOR_SIZE_UP_DOWN:
			SetCursor(LoadCursor(NULL, IDC_SIZENS));
			break;
		case CURSOR_SIZE_LEFT_RIGHT:
			SetCursor(LoadCursor(NULL, IDC_SIZEWE));
			break;
		case CURSOR_SIZE_LEFT_DOWN_CORNER:
			SetCursor(LoadCursor(NULL, IDC_SIZENESW));
			break;
		case CURSOR_SIZE_RIGHT_DOWN_CORNER:
			SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
			break;
	}
}

//-----------------------------------------------------------------------------
std::wstring str2wstr(std::string str) {
	std::wstring ws(str.begin(), str.end());
	return ws;
}

std::wstring key2wstr(KeyType key) {
	wchar_t ws[5] = {};
	BYTE kb[256];
	GetKeyboardState(kb);
	if (ToUnicode(key, MapVirtualKey(key, MAPVK_VK_TO_VSC), kb, ws, 4, 0) > 0) 
	    return std::wstring(ws);
	else
		return std::wstring(L"");
}

//-----------------------------------------------------------------------------
MessageReturnType messageBox(std::wstring text, 
							 std::wstring caption,
							 MessageType type) {
	DWORD style = 0;
	switch (type % MESSAGE_ICON_WARNING) {
		case MESSAGE_ABORT_RETRY_IGNORE:
			style |= MB_ABORTRETRYIGNORE;
			break;
		case MESSAGE_CANCEL_TRY_CONTINUE:
			style |= MB_CANCELTRYCONTINUE;
			break;
		case MESSAGE_HELP:
			style |= MB_HELP;
			break;
		case MESSAGE_OK:
			style |= MB_OK;
			break;
		case MESSAGE_OK_CANCEL:
			style |= MB_OKCANCEL;
			break;
		case MESSAGE_RETRY_CANCEL:
			style |= MB_RETRYCANCEL;
			break;
		case MESSAGE_YES_NO:
			style |= MB_YESNO;
			break;
		case MESSAGE_YES_NO_CANCEL:
			style |= MB_YESNOCANCEL;
			break;
	}
	switch (type / MESSAGE_ICON_WARNING * MESSAGE_ICON_WARNING % MESSAGE_ALIGN_RIGHT_TEXT) {
		case MESSAGE_ICON_WARNING:
			style |= MB_ICONWARNING;
			break;
		case MESSAGE_ICON_INFORMATION:
			style |= MB_ICONINFORMATION;
			break;
		case MESSAGE_ICON_QUESTION:
			style |= MB_ICONQUESTION;
			break;
		case MESSAGE_ICON_ERROR:
			style |= MB_ICONERROR;
			break;
	}
	switch (type / MESSAGE_ALIGN_RIGHT_TEXT * MESSAGE_ALIGN_RIGHT_TEXT % MESSAGE_STYLE_TOPMOST) {
		case MESSAGE_ALIGN_RIGHT_TEXT:
			style |= MB_RIGHT;
			break;
		case MESSAGE_ALIGN_RIGHT_ALL:
			style |= MB_RTLREADING;
			break;
	}
	if (type & MESSAGE_STYLE_TOPMOST)
		style |= MB_TOPMOST;

	switch (MessageBox(NULL, text.c_str(), caption.c_str(), style)) {
		case IDABORT:
			return CLICK_ABORT;
		case IDCANCEL:
			return CLICK_CANCEL;
		case IDCONTINUE:
			return CLICK_CONTINUE;
		case IDIGNORE:
			return CLICK_IGNORE;
		case IDNO:
			return CLICK_NO;
		case IDOK:
			return CLICK_OK;
		case IDRETRY:
			return CLICK_RETRY;
		case IDTRYAGAIN:
			return CLICK_TRYAGAIN;
		case IDYES:
			return CLICK_YES;
	}
}

}