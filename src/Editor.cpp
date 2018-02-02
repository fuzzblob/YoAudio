#include "Editor.h"
#include "GUI.h"
#include "imgui\imgui.h"
#include "../../YoAudio/include/YOAudio.h"

void MenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("(dummy menu)", NULL, false, false);
			if (ImGui::MenuItem("New")) {}
			if (ImGui::MenuItem("Open", "Ctrl+O")) {}
			if (ImGui::BeginMenu("Open Recent"))
			{
				/*ImGui::MenuItem("fish_hat.c");
				ImGui::MenuItem("fish_hat.inl");
				ImGui::MenuItem("fish_hat.h");
				if (ImGui::BeginMenu("More.."))
				{
					ImGui::MenuItem("Hello");
					ImGui::MenuItem("Sailor");
					if (ImGui::BeginMenu("Recurse.."))
					{
						MenuBar();
						ImGui::EndMenu();
					}
					ImGui::EndMenu();
				}*/
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Save", "Ctrl+S")) {}
			if (ImGui::MenuItem("Save As..")) {}
			ImGui::Separator();
			if (ImGui::BeginMenu("Options"))
			{
				/*static bool enabled = true;
				ImGui::MenuItem("Enabled", "", &enabled);
				ImGui::BeginChild("child", ImVec2(0, 60), true);
				for (int i = 0; i < 10; i++)
					ImGui::Text("Scrolling Text %d", i);
				ImGui::EndChild();
				static float f = 0.5f;
				static int n = 0;
				static bool b = true;
				ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
				ImGui::InputFloat("Input", &f, 0.1f);
				ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
				ImGui::Checkbox("Check", &b);
				*/
				ImGui::EndMenu();
			}
			//if (ImGui::BeginMenu("Colors"))
			//{
			//	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			//	for (int i = 0; i < ImGuiCol_COUNT; i++)
			//	{
			//		const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
			//		ImGui::ColorButton(name, ImGui::GetStyleColorVec4((ImGuiCol)i));
			//		ImGui::SameLine();
			//		ImGui::MenuItem(name);
			//	}
			//	ImGui::PopStyleVar();
			//	ImGui::EndMenu();
			//}
			//if (ImGui::BeginMenu("Disabled", false)) // Disabled
			//{
			//	IM_ASSERT(0);
			//}
			//if (ImGui::MenuItem("Checked", NULL, true)) {}
			//
			if (ImGui::MenuItem("Quit", "Alt+F4")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void Editor::Run()
{
	bool quit = false;

	// UI implicitly starts SDL for OpenGL rendering
	std::unique_ptr<Gui> ui = std::make_unique<Gui>();
	std::unique_ptr<Timer> timer = std::make_unique<Timer>();
	std::unique_ptr<InputManager> inputManager = std::make_unique<InputManager>();
	SDL_Event mEvents;
	float deltaTime = 0.0f;

	// init Yo AudioEngine
	if (YOA_Init() == false)
	{
		printf("Yo Audio initialization error! Shutting town");
		quit = true;
	}

	while (quit == false)
	{
		timer->Update();
		deltaTime = timer->DeltaTime();

		while (SDL_PollEvent(&mEvents) != 0)
		{
			ui->ProcessEvent(&mEvents);
			if (mEvents.type == SDL_QUIT)
				quit = true;
		}

		if (deltaTime >= 1.0f / FRAME_RATE)
		{
			inputManager->Update();
			ui->StartFrame();
			// TODO: display GUI label "DeltaTime: %f\n" + deltaTime);

			// main GUI
			App();

			ui->EndFrame();
			timer->Reset();
		}
	}

	// quit Yo audio system
	YOA_Quit(false);
}

void Editor::App()
{
	MenuBar();

	bool bTrue = true;
	{
		//ImGui::Begin("Sound Caster", &bTrue);
		//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		static uint16_t ambLoop = 0u;
		if (ImGui::Button("Play Ambience"))
		{
			ambLoop = YOA_PlayWavFile("ambience.wav", true, 1.0f, 1.0f);
		}
		if (ambLoop != 0u && ImGui::Button("Stop Ambience"))
		{
			// stop the looping sound
			if (YOA_StopVoice(ambLoop) == false)
			{
				fprintf(stderr, "[%s:\t%d]\nError: could not stop amb-loop!\n\n", __FILE__, __LINE__);
			}
			ambLoop = 0u;
		}

		if (ImGui::Button("Play Door Open"))
		{
			YOA_PlayWavFile("door_open_01.wav", false, 0.5f, 1.0f);
		}

		if (ImGui::Button("Play Door Close"))
		{
			YOA_PlayWavFile("door_close_01.wav", false, 0.5f, 1.0f);
		}

		ImGui::Spacing();

		// pause & unpause the audio engine
		if (ImGui::Button("Pause Playback"))
			YOA_Pause();
		
		if (ImGui::Button("Resume Playback"))
			YOA_Resume();
	}
}
