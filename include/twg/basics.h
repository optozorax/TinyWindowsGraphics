#ifndef TWG_BASICS_INCLUDED
#define TWG_BASICS_INCLUDED

#include <windows.h>
#include <stdint.h>
#include <string>

namespace twg
{
	#define TWG_max(a,b) ((a)>(b) ? (a) : (b))
	#define TWG_min(a,b) ((a)<(b) ? (a) : (b))

	typedef int8_t		int8;
	typedef int16_t		int16;
	typedef int32_t		int32; 
	typedef int64_t		int64;
	
	typedef uint8_t		int8u;
	typedef uint16_t	int16u;
	typedef uint32_t	int32u;
	typedef uint64_t	int64u;

	struct Rect;
	enum CycleType : int32u;
	enum KeyType : int32u;
	enum CursorType : int32u;
	enum MessageType : int32u;
	enum MessageReturnType : int32u;

	const double pi = 3.14159265358979323846;

	//-------------------------------------------------------------------------
	struct Rect {
		int32 ax, ay, bx, by;

		Rect(int32 ax, int32 ay, int32 bx, int32 by) : 
			ax(ax), ay(ay), bx(bx), by(by) {}
		Rect() : ax(0), ay(0), bx(0), by(0) {}

		int32 x(void) { return bx-ax; }
		int32 y(void) { return by-ay; }
	};

	//-------------------------------------------------------------------------
	double 	deg2rad(double deg);
	double 	rad2deg(double rad);
	double 	computeCycle(double pos, CycleType type);

	void 	sleep(int32u time);
	bool 	isKeyDown(KeyType key);
	int32u 	timeGet(void);
	void 	setCursor(CursorType type);

	std::wstring str2wstr(std::string str);
	std::wstring key2wstr(KeyType key);

	MessageReturnType messageBox(std::wstring text, 
								 std::wstring caption,
								 MessageType type);

	//-------------------------------------------------------------------------
	enum CycleType : int32u
	{
		CYCLE_NULL, // Обрезается на нуле и на единице
		CYCLE_SIN, // y=x на промежутке [0, 1] повторяется как sin
		CYCLE_TAN // y=x на промежутке [0, 1] повторяется как tg
	};

	enum CursorType : int32u
	{
		CURSOR_NULL,
		CURSOR_STANDART,
		CURSOR_CLICK,
		CURSOR_WAIT,
		CURSOR_STANDART_WAIT,
		CURSOR_CROSS,
		CURSOR_HELP,
		CURSOR_TEXT,
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
		// Типы кнопок
		MESSAGE_ABORT_RETRY_IGNORE 	= 0x1,
		MESSAGE_CANCEL_TRY_CONTINUE = 0x2,
		MESSAGE_HELP				= 0x3,
		MESSAGE_OK 					= 0x4,
		MESSAGE_OK_CANCEL 			= 0x5,
		MESSAGE_RETRY_CANCEL 		= 0x6,
		MESSAGE_YES_NO 				= 0x7,
		MESSAGE_YES_NO_CANCEL 		= 0x8,

		// Иконка
		MESSAGE_ICON_WARNING 		= 0x010,
		MESSAGE_ICON_INFORMATION	= 0x020,
		MESSAGE_ICON_QUESTION		= 0x030,
		MESSAGE_ICON_ERROR			= 0x040,

		// Расположение текста
		MESSAGE_ALIGN_RIGHT_TEXT 	= 0x100,
		MESSAGE_ALIGN_RIGHT_ALL		= 0x200,

		// Общие стили
		MESSAGE_STYLE_TOPMOST 		= 0x400
	};

	enum MessageReturnType : int32u
	{
		CLICK_ABORT,
		CLICK_CANCEL,
		CLICK_CONTINUE,
		CLICK_IGNORE,
		CLICK_NO,
		CLICK_OK,
		CLICK_RETRY,
		CLICK_TRYAGAIN,
		CLICK_YES
	};

	enum KeyType : int32u
	{
		KEY_A = 'A',
		KEY_B = 'B',
		KEY_C = 'C',
		KEY_D = 'D',
		KEY_E = 'E',
		KEY_F = 'F',
		KEY_G = 'G',
		KEY_H = 'H',
		KEY_I = 'I',
		KEY_J = 'J',
		KEY_K = 'K',
		KEY_L = 'L',
		KEY_M = 'M',
		KEY_N = 'N',
		KEY_O = 'O',
		KEY_P = 'P',
		KEY_Q = 'Q',
		KEY_R = 'R',
		KEY_S = 'S',
		KEY_T = 'T',
		KEY_U = 'U',
		KEY_V = 'V',
		KEY_W = 'W',
		KEY_X = 'X',
		KEY_Y = 'Y',
		KEY_Z = 'Z',
		KEY_LBUTTON = VK_LBUTTON,
		KEY_RBUTTON = VK_RBUTTON,
		KEY_CANCEL = VK_CANCEL,
		KEY_MBUTTON = VK_MBUTTON,
		KEY_XBUTTON1 = VK_XBUTTON1,
		KEY_XBUTTON2 = VK_XBUTTON2,
		KEY_BACK = VK_BACK,
		KEY_TAB = VK_TAB,
		KEY_CLEAR = VK_CLEAR,
		KEY_RETURN = VK_RETURN,
		KEY_SHIFT = VK_SHIFT,
		KEY_CONTROL = VK_CONTROL,
		KEY_MENU = VK_MENU,
		KEY_PAUSE = VK_PAUSE,
		KEY_CAPITAL = VK_CAPITAL,
		KEY_KANA = VK_KANA,
		KEY_HANGEUL = VK_HANGEUL,
		KEY_HANGUL = VK_HANGUL,
		KEY_JUNJA = VK_JUNJA,
		KEY_FINAL = VK_FINAL,
		KEY_HANJA = VK_HANJA,
		KEY_KANJI = VK_KANJI,
		KEY_ESCAPE = VK_ESCAPE,
		KEY_CONVERT = VK_CONVERT,
		KEY_NONCONVERT = VK_NONCONVERT,
		KEY_ACCEPT = VK_ACCEPT,
		KEY_MODECHANGE = VK_MODECHANGE,
		KEY_SPACE = VK_SPACE,
		KEY_PRIOR = VK_PRIOR,
		KEY_NEXT = VK_NEXT,
		KEY_END = VK_END,
		KEY_HOME = VK_HOME,
		KEY_LEFT = VK_LEFT,
		KEY_UP = VK_UP,
		KEY_RIGHT = VK_RIGHT,
		KEY_DOWN = VK_DOWN,
		KEY_SELECT = VK_SELECT,
		KEY_PRINT = VK_PRINT,
		KEY_EXEC = VK_EXECUTE,
		KEY_SNAPSHOT = VK_SNAPSHOT,
		KEY_INSERT = VK_INSERT,
		KEY_DELETE = VK_DELETE,
		KEY_HELP = VK_HELP,
		KEY_LWIN = VK_LWIN,
		KEY_RWIN = VK_RWIN,
		KEY_APPS = VK_APPS,
		KEY_SLEEP = VK_SLEEP,
		KEY_NUMPAD0 = VK_NUMPAD0,
		KEY_NUMPAD1 = VK_NUMPAD1,
		KEY_NUMPAD2 = VK_NUMPAD2,
		KEY_NUMPAD3 = VK_NUMPAD3,
		KEY_NUMPAD4 = VK_NUMPAD4,
		KEY_NUMPAD5 = VK_NUMPAD5,
		KEY_NUMPAD6 = VK_NUMPAD6,
		KEY_NUMPAD7 = VK_NUMPAD7,
		KEY_NUMPAD8 = VK_NUMPAD8,
		KEY_NUMPAD9 = VK_NUMPAD9,
		KEY_MULTIPLY = VK_MULTIPLY,
		KEY_ADD = VK_ADD,
		KEY_SEPARATOR = VK_SEPARATOR,
		KEY_SUBTRACT = VK_SUBTRACT,
		KEY_DECIMAL = VK_DECIMAL,
		KEY_DIVIDE = VK_DIVIDE,
		KEY_F1 = VK_F1,
		KEY_F2 = VK_F2,
		KEY_F3 = VK_F3,
		KEY_F4 = VK_F4,
		KEY_F5 = VK_F5,
		KEY_F6 = VK_F6,
		KEY_F7 = VK_F7,
		KEY_F8 = VK_F8,
		KEY_F9 = VK_F9,
		KEY_F10 = VK_F10,
		KEY_F11 = VK_F11,
		KEY_F12 = VK_F12,
		KEY_F13 = VK_F13,
		KEY_F14 = VK_F14,
		KEY_F15 = VK_F15,
		KEY_F16 = VK_F16,
		KEY_F17 = VK_F17,
		KEY_F18 = VK_F18,
		KEY_F19 = VK_F19,
		KEY_F20 = VK_F20,
		KEY_F21 = VK_F21,
		KEY_F22 = VK_F22,
		KEY_F23 = VK_F23,
		KEY_F24 = VK_F24,
		KEY_NUMLOCK = VK_NUMLOCK,
		KEY_SCROLL = VK_SCROLL,
		KEY_LSHIFT = VK_LSHIFT,
		KEY_RSHIFT = VK_RSHIFT,
		KEY_LCONTROL = VK_LCONTROL,
		KEY_RCONTROL = VK_RCONTROL,
		KEY_LMENU = VK_LMENU,
		KEY_RMENU = VK_RMENU,
		KEY_BROWSER_BACK = VK_BROWSER_BACK,
		KEY_BROWSER_FORWARD = VK_BROWSER_FORWARD,
		KEY_BROWSER_REFRESH = VK_BROWSER_REFRESH,
		KEY_BROWSER_STOP = VK_BROWSER_STOP,
		KEY_BROWSER_SEARCH = VK_BROWSER_SEARCH,
		KEY_BROWSER_FAVORITES = VK_BROWSER_FAVORITES,
		KEY_BROWSER_HOME = VK_BROWSER_HOME,
		KEY_VOLUME_MUTE = VK_VOLUME_MUTE,
		KEY_VOLUME_DOWN = VK_VOLUME_DOWN,
		KEY_VOLUME_UP = VK_VOLUME_UP,
		KEY_MEDIA_NEXT_TRACK = VK_MEDIA_NEXT_TRACK,
		KEY_MEDIA_PREV_TRACK = VK_MEDIA_PREV_TRACK,
		KEY_MEDIA_STOP = VK_MEDIA_STOP,
		KEY_MEDIA_PLAY_PAUSE = VK_MEDIA_PLAY_PAUSE,
		KEY_LAUNCH_MAIL = VK_LAUNCH_MAIL,
		KEY_LAUNCH_MEDIA_SELECT = VK_LAUNCH_MEDIA_SELECT,
		KEY_LAUNCH_APP1 = VK_LAUNCH_APP1,
		KEY_LAUNCH_APP2 = VK_LAUNCH_APP2,
		KEY_OEM_1 = VK_OEM_1,
		KEY_OEM_PLUS = VK_OEM_PLUS,
		KEY_OEM_COMMA = VK_OEM_COMMA,
		KEY_OEM_MINUS = VK_OEM_MINUS,
		KEY_OEM_PERIOD = VK_OEM_PERIOD,
		KEY_OEM_2 = VK_OEM_2,
		KEY_OEM_3 = VK_OEM_3,
		KEY_OEM_4 = VK_OEM_4,
		KEY_OEM_5 = VK_OEM_5,
		KEY_OEM_6 = VK_OEM_6,
		KEY_OEM_7 = VK_OEM_7,
		KEY_OEM_8 = VK_OEM_8,
		KEY_OEM_102 = VK_OEM_102,
		KEY_PROCESSKEY = VK_PROCESSKEY,
		KEY_PACKET = VK_PACKET,
		KEY_ATTN = VK_ATTN,
		KEY_CRSEL = VK_CRSEL,
		KEY_EXSEL = VK_EXSEL,
		KEY_EREOF = VK_EREOF,
		KEY_PLAY = VK_PLAY,
		KEY_ZOOM = VK_ZOOM,
		KEY_NONAME = VK_NONAME,
		KEY_PA1 = VK_PA1,
		KEY_OEM_CLEAR = VK_OEM_CLEAR
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

}

#endif // TWG_BASICS_INCLUDED