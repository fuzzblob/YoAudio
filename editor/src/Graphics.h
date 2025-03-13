#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "SDLopenGL.h"
#include <stdio.h>
#include <string>
#include <memory>

class Graphics
{
private:
	std::unique_ptr<SDLopenGL> mImplementation;
public:
	SDL_Window* GetWindow() noexcept {
		return mImplementation->GetWindow(); 
	};
	Graphics() noexcept
	{
		mImplementation = std::make_unique<SDLopenGL>();
	};
	~Graphics() { mImplementation = nullptr; };

	void StartFrame() { mImplementation->StartFrame(); };
	void EndFrame() { mImplementation->EndFrame(); };
	bool ProcessEvent(SDL_Event* event)
	{ return mImplementation->ProcessEvent(event); };
};

#endif // !_GRAPHICS_H