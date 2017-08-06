#include <iostream>
#include <cstdlib>
#include "twg/twg.h"
#include "twg/window/window_events.h"

using namespace twg;

class MyWindow : public WindowEvents
{
public:
	MyWindow(WindowType type) : WindowEvents(type) {}

	bool onMouse(Point_i pos, MouseType type) {
		std::system("cls");
		std::cout << "Mouse type: ";
		switch (type) {
			case MOUSE_L_DBL:		std::cout << "MOUSE_L_DBL"; break;
			case MOUSE_L_DOWN:		std::cout << "MOUSE_L_DOWN"; break;
			case MOUSE_L_UP:		std::cout << "MOUSE_L_UP"; break;
			case MOUSE_R_DBL:		std::cout << "MOUSE_R_DBL"; break;
			case MOUSE_R_DOWN:		std::cout << "MOUSE_R_DOWN"; break;
			case MOUSE_R_UP:		std::cout << "MOUSE_R_UP"; break;
			case MOUSE_M_DBL:		std::cout << "MOUSE_M_DBL"; break;
			case MOUSE_M_DOWN:		std::cout << "MOUSE_M_DOWN"; break;
			case MOUSE_M_UP:		std::cout << "MOUSE_M_UP"; break;
			case MOUSE_X1_DBL:		std::cout << "MOUSE_X1_DBL"; break;
			case MOUSE_X1_DOWN:		std::cout << "MOUSE_X1_DOWN"; break;
			case MOUSE_X1_UP:		std::cout << "MOUSE_X1_UP"; break;
			case MOUSE_X2_DBL:		std::cout << "MOUSE_X2_DBL"; break;
			case MOUSE_X2_DOWN:		std::cout << "MOUSE_X2_DOWN"; break;
			case MOUSE_X2_UP:		std::cout << "MOUSE_X2_UP"; break;
			case MOUSE_WHEEL_UP:	std::cout << "MOUSE_WHEEL_UP"; break;
			case MOUSE_WHEEL_DOWN:	std::cout << "MOUSE_WHEEL_DOWN"; break;
			case MOUSE_MOVE:		std::cout << "MOUSE_MOVE"; break;
		}
		std::cout << "\n\tat: (" << pos.x << ", " << pos.y << ");" << std::endl;
		return true;
	}

	bool onKeyboard(KeyType key, bool isDown) {
		std::system("cls");
		std::cout << "Pressed ";
		#define write(A) if (key == A) std::cout << #A; else
		write(KEY_A)
		write(KEY_B)
		write(KEY_C)
		write(KEY_D)
		write(KEY_E)
		write(KEY_F)
		write(KEY_G)
		write(KEY_H)
		write(KEY_I)
		write(KEY_J)
		write(KEY_K)
		write(KEY_L)
		write(KEY_M)
		write(KEY_N)
		write(KEY_O)
		write(KEY_P)
		write(KEY_Q)
		write(KEY_R)
		write(KEY_S)
		write(KEY_T)
		write(KEY_U)
		write(KEY_V)
		write(KEY_W)
		write(KEY_X)
		write(KEY_Y)
		write(KEY_Z)
		write(KEY_LBUTTON)
		write(KEY_RBUTTON)
		write(KEY_CANCEL)
		write(KEY_MBUTTON)
		write(KEY_XBUTTON1)
		write(KEY_XBUTTON2)
		write(KEY_BACK)
		write(KEY_TAB)
		write(KEY_CLEAR)
		write(KEY_RETURN)
		write(KEY_SHIFT)
		write(KEY_CONTROL)
		write(KEY_MENU)
		write(KEY_PAUSE)
		write(KEY_CAPITAL)
		write(KEY_KANA)
		write(KEY_HANGEUL)
		write(KEY_HANGUL)
		write(KEY_JUNJA)
		write(KEY_FINAL)
		write(KEY_HANJA)
		write(KEY_KANJI)
		write(KEY_ESCAPE)
		write(KEY_CONVERT)
		write(KEY_NONCONVERT)
		write(KEY_ACCEPT)
		write(KEY_MODECHANGE)
		write(KEY_SPACE)
		write(KEY_PRIOR)
		write(KEY_NEXT)
		write(KEY_END)
		write(KEY_HOME)
		write(KEY_LEFT)
		write(KEY_UP)
		write(KEY_RIGHT)
		write(KEY_DOWN)
		write(KEY_SELECT)
		write(KEY_PRINT)
		write(KEY_EXEC)
		write(KEY_SNAPSHOT)
		write(KEY_INSERT)
		write(KEY_DELETE)
		write(KEY_HELP)
		write(KEY_LWIN)
		write(KEY_RWIN)
		write(KEY_APPS)
		write(KEY_SLEEP)
		write(KEY_NUMPAD0)
		write(KEY_NUMPAD1)
		write(KEY_NUMPAD2)
		write(KEY_NUMPAD3)
		write(KEY_NUMPAD4)
		write(KEY_NUMPAD5)
		write(KEY_NUMPAD6)
		write(KEY_NUMPAD7)
		write(KEY_NUMPAD8)
		write(KEY_NUMPAD9)
		write(KEY_MULTIPLY)
		write(KEY_ADD)
		write(KEY_SEPARATOR)
		write(KEY_SUBTRACT)
		write(KEY_DECIMAL)
		write(KEY_DIVIDE)
		write(KEY_F1)
		write(KEY_F2)
		write(KEY_F3)
		write(KEY_F4)
		write(KEY_F5)
		write(KEY_F6)
		write(KEY_F7)
		write(KEY_F8)
		write(KEY_F9)
		write(KEY_F10)
		write(KEY_F11)
		write(KEY_F12)
		write(KEY_F13)
		write(KEY_F14)
		write(KEY_F15)
		write(KEY_F16)
		write(KEY_F17)
		write(KEY_F18)
		write(KEY_F19)
		write(KEY_F20)
		write(KEY_F21)
		write(KEY_F22)
		write(KEY_F23)
		write(KEY_F24)
		write(KEY_NUMLOCK)
		write(KEY_SCROLL)
		write(KEY_LSHIFT)
		write(KEY_RSHIFT)
		write(KEY_LCONTROL)
		write(KEY_RCONTROL)
		write(KEY_LMENU)
		write(KEY_RMENU)
		write(KEY_BROWSER_BACK)
		write(KEY_BROWSER_FORWARD)
		write(KEY_BROWSER_REFRESH)
		write(KEY_BROWSER_STOP)
		write(KEY_BROWSER_SEARCH)
		write(KEY_BROWSER_FAVORITES)
		write(KEY_BROWSER_HOME)
		write(KEY_VOLUME_MUTE)
		write(KEY_VOLUME_DOWN)
		write(KEY_VOLUME_UP)
		write(KEY_MEDIA_NEXT_TRACK)
		write(KEY_MEDIA_PREV_TRACK)
		write(KEY_MEDIA_STOP)
		write(KEY_MEDIA_PLAY_PAUSE)
		write(KEY_LAUNCH_MAIL)
		write(KEY_LAUNCH_MEDIA_SELECT)
		write(KEY_LAUNCH_APP1)
		write(KEY_LAUNCH_APP2)
		write(KEY_OEM_1)
		write(KEY_OEM_PLUS)
		write(KEY_OEM_COMMA)
		write(KEY_OEM_MINUS)
		write(KEY_OEM_PERIOD)
		write(KEY_OEM_2)
		write(KEY_OEM_3)
		write(KEY_OEM_4)
		write(KEY_OEM_5)
		write(KEY_OEM_6)
		write(KEY_OEM_7)
		write(KEY_OEM_8)
		write(KEY_OEM_102)
		write(KEY_PROCESSKEY)
		write(KEY_PACKET)
		write(KEY_ATTN)
		write(KEY_CRSEL)
		write(KEY_EXSEL)
		write(KEY_EREOF)
		write(KEY_PLAY)
		write(KEY_ZOOM)
		write(KEY_NONAME)
		write(KEY_PA1)
		write(KEY_OEM_CLEAR)
		std::cout << "key with scan-code " << key << std::endl;
		return true;
	}

	bool onResize(Point_i newSize, 
				  Point_i pos, 
				  SizingType type) {
		std::system("cls");

		std::cout << "Resize type: ";
		switch (type) {
			case SIZING_LEFT:			std::cout << "SIZING_LEFT";			break;
			case SIZING_RIGHT:			std::cout << "SIZING_RIGHT";		break;
			case SIZING_BOTTOM:			std::cout << "SIZING_BOTTOM";		break;
			case SIZING_TOP:			std::cout << "SIZING_TOP";			break;
			case SIZING_BOTTOM_LEFT:	std::cout << "SIZING_BOTTOM_LEFT";	break;
			case SIZING_BOTTOM_RIGHT:	std::cout << "SIZING_BOTTOM_RIGHT";	break;
			case SIZING_TOP_LEFT:		std::cout << "SIZING_TOP_LEFT";		break;
			case SIZING_TOP_RIGHT:		std::cout << "SIZING_TOP_RIGHT";	break;
		}
		std::cout << "\n\t at: (" << pos.x << ", " << pos.y << ");" << std::endl;
		std::cout << "       size: (" << newSize.x << ", " << newSize.y << ");" << std::endl;
		return true;
	}

	bool onMove(Point_i newPos) {
		std::system("cls");
		std::cout << "Move to (" << newPos.x << ", " << newPos.y << ");" << std::endl;
		return true;
	}

	bool onKillFocus(void) {
		std::system("cls");
		std::cout << "Kill focus." << std::endl;
		return true;
	}

	bool onMessage(int32u messageNo, void* data) {
		delete data;
		return false;
	}
};

int main() {
	WindowType type(-1,
		Point_i(100, 0),
		Point_i(300, 300), 
		Point_i(-1, -1),
		Point_i(-1, -1),
		L"Test events of WindowEvents class.",
		WINDOW_STANDART);	
	MyWindow wnd(type);
	wnd.waitForClose();
	return 0;
}