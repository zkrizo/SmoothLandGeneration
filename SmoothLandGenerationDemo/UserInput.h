#ifndef UserInput_H
#define UserInput_H
#include <forward_list>

class UserInput
{
public:
	static UserInput* Instance();
	void Update(const float& time);

	void KeyPress(const bool& value,const int& key);
	void MouseMove(const int& x, const int& y);
	bool isPressed(const int& key);
	int getMouseX(){return mouseX;};
	int getMouseY(){return mouseY;};

private:
	UserInput();
	~UserInput();
	UserInput(const UserInput&){};
	UserInput operator= (const UserInput&){};
	int mouseX;
	int mouseY;

	//contains a list of all keys and buttons that are pressed down
	std::forward_list<int> keysDown;
};

enum KEY
{
	KEY_A=0,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_MINUS,
	KEY_PLUS,
	KEY_BACKSPACE,
	KEY_TILDA,
	KEY_TAB,
	KEY_CAPS,
	KEY_SHIFT,
	KEY_CTRL,
	KEY_ALT,
	KEY_ENTER,
	KEY_SPACE,
	KEY_UPARROW,
	KEY_DOWNARROW,
	KEY_LEFTARROW,
	KEY_RIGHTARROW,
	KEY_ESC,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_NUMLOCK,
	KEY_NUMSLASH,
	KEY_NUMMULTIPLY,
	KEY_NUMMINUS,
	KEY_NUMPLUS,
	KEY_NUMENTER,
	KEY_NUMPERIOD,
	KEY_NUM0,
	KEY_NUM1,
	KEY_NUM2,
	KEY_NUM3,
	KEY_NUM4,
	KEY_NUM5,
	KEY_NUM6,
	KEY_NUM7,
	KEY_NUM8,
	KEY_NUM9,
	KEY_MOUSELEFT,
	KEY_MOUSERIGHT,
	KEY_MOUSEMIDDLE,

};

#endif