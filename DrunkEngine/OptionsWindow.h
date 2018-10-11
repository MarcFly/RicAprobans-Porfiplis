#ifndef _OPTIONSWINDOW_H_
#define _OPTIONSWINDOW_H_

#include "Window.h"
#include "Application.h"
#include <vector>

class OptionsWindow : public Window
{
public:
	OptionsWindow();
	virtual ~OptionsWindow();

	void Draw() override;

	void CheckEnableDisableOpenGL();

	void CheckInputChange();

public:

	float fps;
	float max_fps;

	Timer frame_read_time;
	Timer repeated_key_time;

	int fps_h_size = 25;
	int dt_h_size = 60;
	std::vector<float> fps_history;
	std::vector<float> dt_history;

	int input_change;
	bool key_repeated;

};

#endif
