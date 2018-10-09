#include "Globals.h"
#include "Module.h"
#include "ModuleUI.h"
#include "ModuleInput.h"
#include "Application.h"
#include "imgui/imgui.h"
#include "imgui/implements/imgui_impl_sdl.h"
#include "imgui/implements/imgui_impl_opengl2.h"
#include <gl/GL.h>
#include <string.h>
#include "Window.h"
#include "OptionsWindow.h"
#include "AboutWindow.h"
#include "RandomGenWindow.h"
#include "GeometryCreationWindow.h"
#include "OpenGLOptions.h"
#include "ConsoleWindow.h"
#include "GeoTransformWindow.h"

#define MEM_BUDGET_NVX 0x9048
#define MEM_AVAILABLE_NVX 0x9049

using namespace std;

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ImGui::CreateContext();
}

ModuleUI::~ModuleUI()
{
}

bool ModuleUI::Init()
{
	PLOG("Initiating IMGUI Module")
	bool ret = true;

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();
	ImGui::StyleColorsDark();

	show_demo_window = false;

	windows.push_back(options_win = new OptionsWindow(App));
	windows.push_back(about_win = new AboutWindow(App));
	windows.push_back(random_win = new RandomGenWindow());
	windows.push_back(geometry_win = new GEOMWindow(App));
	windows.push_back(opengl_win = new OpenGLOptions(App));
	windows.push_back(console_win = new ConsoleWindow());
	windows.push_back(geo_transform_win = new GeoTransformWindow(App));

	return ret;
}

update_status ModuleUI::PreUpdate(float dt)
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	MainMenu();

	for (vector<Window*>::iterator it = windows.begin(); it != windows.end(); ++it)
	{
		Window* windows = (*it);

		if (App->input->GetKey(windows->GetShortCut()) == KEY_DOWN)
			windows->SwitchActive();

		if (windows->IsActive())
			windows->Draw();
	}

	return UPDATE_CONTINUE;
}

update_status ModuleUI::PostUpdate(float dt)
{
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	return UPDATE_CONTINUE;
}

bool ModuleUI::CleanUp()
{
	PLOG("Cleaning UP IMGUI Module")
	bool ret = true;
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return ret;
}

bool ModuleUI::MainMenu()
{
	bool ret = true;

	ImGui::BeginMainMenuBar();
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::MenuItem("Options"))
				options_win->SwitchActive();

			if (ImGui::MenuItem("OpenGL Settings"))
				opengl_win->SwitchActive();

			if (ImGui::MenuItem("Console"))
				console_win->SwitchActive();

			if (ImGui::MenuItem("Exit"))
				App->input->StopRunning();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools"))
		{
			if (ImGui::MenuItem("Random Generator"))
				random_win->SwitchActive();

			if (ImGui::MenuItem("Test Objects"))
				geometry_win->SwitchActive();

			if (ImGui::MenuItem("Transform Meshes"))
				geo_transform_win->SwitchActive();

			if (ImGui::MenuItem("Hide All")) {
				for (int i = 0; i < windows.size(); i++)
					windows[i]->SetInactive();

				show_demo_window = false;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Examples"))
				show_demo_window = !show_demo_window;

			if (ImGui::MenuItem("Download Latest"))
				ShellExecute(NULL, "open", "https://github.com/MarcFly/DrunkEngine/releases", NULL, NULL, SW_SHOWNORMAL);

			if (ImGui::MenuItem("Documentation"))
				ShellExecute(NULL, "open", "https://github.com/MarcFly/DrunkEngine", NULL, NULL, SW_SHOWNORMAL);

			if (ImGui::MenuItem("Report a Bug"))
				ShellExecute(NULL, "open", "https://github.com/MarcFly/DrunkEngine/issues", NULL, NULL, SW_SHOWNORMAL);
						
			if (ImGui::MenuItem("About..."))
				about_win->SwitchActive();

			ImGui::EndMenu();
		}

		
		
	}
	ImGui::EndMainMenuBar();

	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	return ret;
}
