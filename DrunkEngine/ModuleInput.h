#ifndef _MODULE_INPUT_H_
#define _MODULE_INPUT_H_

#include "Module.h"
#include "Globals.h"

#define MAX_MOUSE_BUTTONS 5

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

enum Controls {

	MOVE_FORWARD,
	MOVE_BACK,
	MOVE_LEFT,
	MOVE_RIGHT,
	FOCUS_CAMERA,
	ORBIT_CAMERA,
	OPTIONS_MENU,
	CONSOLE_MENU,
	MESH_MENU,
	ABOUT_MENU,
	NULL_CONTROL
};

class ModuleInput : public Module
{
public:
	
	ModuleInput(bool start_enabled = true);
	~ModuleInput();

	bool Init();
	update_status PreUpdate(float dt);
	bool CleanUp();

	KEY_STATE GetKey(int id) const
	{
		return keyboard[id];
	}

	KEY_STATE GetMouseButton(int id) const
	{
		return mouse_buttons[id];
	}

	int GetMouseX() const
	{
		return mouse_x;
	}

	int GetMouseY() const
	{
		return mouse_y;
	}

	int GetMouseZ() const
	{
		return mouse_z;
	}

	int GetMouseXMotion() const
	{
		return mouse_x_motion;
	}

	int GetMouseYMotion() const
	{
		return mouse_y_motion;
	}

	void StopRunning()
	{
		quit = true;
	}

	void UpdateShortcuts();
	void SetDefaultControls();

	bool Load(JSON_Value* root_value);
	bool Save(JSON_Value* root_value);

	bool CheckExtension(std::string& ext);
	void LoadFile();

private:
	KEY_STATE* keyboard;
	KEY_STATE mouse_buttons[MAX_MOUSE_BUTTONS];
	int mouse_x;
	int mouse_y;
	int mouse_z;
	int mouse_x_motion;
	int mouse_y_motion;
	//int mouse_z_motion;

public:

	int controls[NULL_CONTROL];

	bool quit;

private:
	void SetDefault()
	{
		controls[MOVE_FORWARD]	= 26;
		controls[MOVE_BACK]		= 22;
		controls[MOVE_LEFT]		= 4;
		controls[MOVE_RIGHT]	= 7;
		controls[FOCUS_CAMERA]	= 9;
		controls[ORBIT_CAMERA]	= 226;
		controls[OPTIONS_MENU]	= 18;
		controls[CONSOLE_MENU]	= 6;
		controls[MESH_MENU]		= 16;
		controls[ABOUT_MENU]	= 12;
		
	}
};

#endif