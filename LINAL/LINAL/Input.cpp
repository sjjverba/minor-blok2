#include "Input.h"
#include "Viewer.h"
#include <SDL_events.h>
#include "Events.h"

void Input::handleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
		case SDL_QUIT:
			Viewer::running = false;
			break;
		case SDL_WINDOWEVENT: 
			switch (event.window.event)
			{
				case SDL_WINDOWEVENT_RESIZED:
					Viewer::onWindowResize.call();
					break;
			}
		default:
			break;
	}
}
