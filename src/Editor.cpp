#include "Editor.h"
#include "InputManager.h"
#include "GUI.h"
#include "imgui\imgui.h"

#include "Timer.h"
#include "YoAudio.h"

void Editor::Run()
{
	// UI implicitly starts SDL for OpenGL rendering
	std::unique_ptr<Gui> ui = std::make_unique<Gui>();
	mTimer = std::make_unique<Timer>();
	std::unique_ptr<InputManager> inputManager = std::make_unique<InputManager>();
	SDL_Event mEvents;
	float deltaTime = 0.0f;

	// init Yo AudioEngine
	if (YOA_Init() == false)
	{
		printf("Yo Audio initialization error! Shutting town\n");
		mQuit = true;
	}

	while (mQuit == false)
	{
		while (SDL_PollEvent(&mEvents) != 0)
		{
			ui->ProcessEvent(&mEvents);
			if (mEvents.type == SDL_QUIT)
				mQuit = true;
		}
		// reset deltaTime
		mTimer->ResetDeltaTime();
		// wait for frame rate
		const double targetFrameLength = 1.0 / FRAME_RATE;
		while (mTimer->DeltaTime() < targetFrameLength) {
			mTimer->Update();
		}
		inputManager->Update();
		// begin GUI
		ui->StartFrame();
		// main GUI
		App();
		// end GUI
		ui->EndFrame();
	}

	// quit Yo audio system
	YOA_Quit(false);
}

void Editor::App()
{
	Menu();

	bool bTrue = true;
	{
		//ImGui::Begin("Sound Caster", &bTrue);
		//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		
		static float volume = 1.0f;
		static float pitch = 1.0f;
		
		ImGui::SliderFloat("volume", &volume, 0.0f, 1.0f);

		ImGui::SliderFloat("pitch", &pitch, 0.0f, 4.0f);

		static uint16_t ambLoop_01 = 0u;
		if (ambLoop_01 == 0u && ImGui::Button("Play Ambience 01"))
		{
			ambLoop_01 = YOA_PlayWavFile("ambience_01.wav", true, 1.0f * volume, 1.0f * pitch, 12.0f);
		}
		if (ambLoop_01 != 0u && ImGui::Button("Stop Ambience 01"))
		{
			// stop the looping sound
			if (YOA_StopVoice(ambLoop_01, 2.5f) != 1)
			{
				printf("Error: could not stop amb-loop!\n");
			}
			ambLoop_01 = 0u;
		}

		static uint16_t ambLoop_02 = 0u;
		if (ambLoop_02 == 0u && ImGui::Button("Play Ambience 02"))
		{
			ambLoop_02 = YOA_PlayWavFile("ambience_02.wav", true, 1.0f * volume, 1.0f * pitch, 5.0f);
		}
		if (ambLoop_02 != 0u && ImGui::Button("Stop Ambience 02"))
		{
			// stop the looping sound
			if (YOA_StopVoice(ambLoop_02, 0.0f) != 1)
			{
				printf("Error: could not stop amb-loop!\n");
			}
			ambLoop_02 = 0u;
		}

		static uint16_t engineLoop = 0u;
		if (engineLoop == 0u && ImGui::Button("Play Engine"))
		{
			engineLoop = YOA_PlayWavFile("engine.wav", true, 1.0f * volume, 1.0f * pitch, 5.0f);
		}
		if (engineLoop != 0u && ImGui::Button("Stop Engine"))
		{
			// stop the looping sound
			if (YOA_StopVoice(engineLoop, 0.0f) != 1)
			{
				printf("Error: could not stop Engine!\n");
			}
			engineLoop = 0u;
		}

		if (ImGui::Button("Play Door Open"))
		{
			YOA_PlayWavFile("door_open_01.wav", false, 0.5f * volume, 1.0f * pitch, 0.0f);
		}

		if (ImGui::Button("Play Door Close"))
		{
			YOA_PlayWavFile("door_close_01.wav", false, 0.5f * volume, 1.0f * pitch, 0.0f);
		}

		ImGui::Spacing();

		// pause & unpause the audio engine
		if (ImGui::Button("Pause Playback"))
			YOA_Pause();
		
		if (ImGui::Button("Resume Playback"))
			YOA_Resume();
	}
}

void Editor::Menu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("(dummy menu)", nullptr, false, false);
			//if (ImGui::MenuItem("New")) {}
			//if (ImGui::MenuItem("Open", "Ctrl+O")) {}
			//if (ImGui::BeginMenu("Open Recent"))
			//{
			//	/*ImGui::MenuItem("fish_hat.c");
			//	ImGui::MenuItem("fish_hat.inl");
			//	ImGui::MenuItem("fish_hat.h");
			//	if (ImGui::BeginMenu("More.."))
			//	{
			//		ImGui::MenuItem("Hello");
			//		ImGui::MenuItem("Sailor");
			//		if (ImGui::BeginMenu("Recurse.."))
			//		{
			//			MenuBar();
			//			ImGui::EndMenu();
			//		}
			//		ImGui::EndMenu();
			//	}*/
			//	ImGui::EndMenu();
			//}
			//if (ImGui::MenuItem("Save", "Ctrl+S")) {}
			//if (ImGui::MenuItem("Save As..")) {}
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
			if (ImGui::MenuItem("Quit", "Alt+F4"))
			{
				mQuit = true;
			}
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

		ImGui::Separator();

		float delta = this->mTimer->DeltaTime();
		ImGui::InputFloat("FPS: ", &delta);
		
		ImGui::EndMainMenuBar();
	}
}
