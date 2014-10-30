#include "UserInput.h"
#include "Singletons.h"

UserInput* UserInput::Instance()
{
	static UserInput instance;
	return &instance;
}

/****************************
* Constructor
****************************/
UserInput::UserInput()
{
}

/****************************
* Destructor
****************************/
UserInput::~UserInput()
{
}

/****************************
* Update
****************************/
void UserInput::Update(const float& time)
{

	//checks each of the keys that are down
	std::forward_list<int>::iterator iterator;
	iterator=keysDown.begin();


	if(!keysDown.empty())
	{
		//continues until the end of the list of keys held down
		while(iterator!=keysDown.end())
		{
			if(KEY_W==*iterator)
			{
				ogl->camera.addTranslationCamera(glm::vec3(0,0,-1),time);
			}
			else if(KEY_A==*iterator)
			{
				ogl->camera.addTranslationCamera(glm::vec3(-1,0,0),time);
			}
			else if(KEY_S==*iterator)
			{
				ogl->camera.addTranslationCamera(glm::vec3(0,0,1),time);
			}
			else if(KEY_D==*iterator)
			{
				ogl->camera.addTranslationCamera(glm::vec3(1,0,0),time);
			}
			else if(KEY_SPACE==*iterator)
			{
				ogl->camera.addTranslationCamera(glm::vec3(0,1,0),time);
			}
			else if(KEY_SHIFT==*iterator)
			{
				ogl->camera.addTranslationCamera(glm::vec3(0,-1,0),time);
			}
				
			iterator++;
		};
	}
}

/****************************
* MouseMove
****************************/
void UserInput::MouseMove(const int& x, const int& y)
{
	if(isPressed(KEY_MOUSERIGHT))
	{
		ogl->camera.addRotationToCamera(0.25f*(x-mouseX),0.25f*(y-mouseY));
	}
	mouseX=x;
	mouseY=y;
}

/****************************
* KeyPress
****************************/
void UserInput::KeyPress(const bool& value, const int& key)
{
	switch(key)
	{
	case 0:   //Key 'A'
		if(value)
			keysDown.push_front(KEY_A);
		else
			keysDown.remove(KEY_A);
		break;
	case 3:   //Key 'D'
		if(value)
			keysDown.push_front(KEY_D);
		else
			keysDown.remove(KEY_D);
		break;
	case 18:   //Key 'S'
		if(value)
			keysDown.push_front(KEY_S);
		else
			keysDown.remove(KEY_S);
		break;
	case 22:   //Key 'W'
		if(value)
			keysDown.push_front(KEY_W);
		else
			keysDown.remove(KEY_W);
		break;
	case 42:   //Key 'SHIFT'
		if(value)
			keysDown.push_front(KEY_SHIFT);
		else
			keysDown.remove(KEY_SHIFT);
		break;
	case 46:   //Key 'SPACE'
		if(value)
			keysDown.push_front(KEY_SPACE);
		else
			keysDown.remove(KEY_SPACE);
		break;
	case 47:   //Key 'UPARROW'
		if(value)
			keysDown.push_front(KEY_UPARROW);
		else
			keysDown.remove(KEY_UPARROW);
		break;
	case 48:   //Key 'DOWNARROW'
		if(value)
			keysDown.push_front(KEY_DOWNARROW);
		else
			keysDown.remove(KEY_DOWNARROW);
		break;
	case 49:   //Key 'LEFTARROW'
		if(value)
			keysDown.push_front(KEY_LEFTARROW);
		else
			keysDown.remove(KEY_LEFTARROW);
		break;
	case 50:	//Key 'RIGHTARROW
		if(value)
			keysDown.push_front(KEY_RIGHTARROW);
		else
			keysDown.remove(KEY_RIGHTARROW);
		break;
	case 51:	//Key 'ESC'
		if(value)
			keysDown.push_front(KEY_ESC);
		else
			keysDown.remove(KEY_ESC);
		break;
	case 82:	//Key 'MOUSERIGHT'
		if(value)
			keysDown.push_front(KEY_MOUSERIGHT);
		else
			keysDown.remove(KEY_MOUSERIGHT);
		break;
	}
}

/****************************
* isPressed
****************************/
bool UserInput::isPressed(const int& key)
{
	std::forward_list<int>::iterator iterator;
	iterator=keysDown.begin();

	if(keysDown.empty())
	{
		return false;
	}
	else
	{
		while(iterator!=keysDown.end())
		{
			if(*iterator==key)
				return true;
			iterator++;
		};
	}

	return false;
}