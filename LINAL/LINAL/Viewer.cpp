#include "Viewer.h"
#include <memory>
#include <SDL.h>
#include "CoordinateSystem.h"
#include "Line.h"

event<> Viewer::onWindowResize;
SDL_Renderer* Viewer::renderer = nullptr;
SDL_Window* Viewer::window = nullptr;
bool Viewer::running = true;
Color Viewer::drawColor {255,255,255,255};
int Viewer::screenHeight = 0;
int Viewer::screenWidth = 0;

Viewer::Viewer()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	
    window = SDL_CreateWindow("LINAL", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		500,
		500,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, drawColor.r, drawColor.g, drawColor.b, drawColor.a);

	entities.push_back(std::make_unique<CoordinateSystem>(10,10));

	startLoop();
}

void Viewer::startLoop()
{
	while (running)
	{
		input.handleEvents();
		update();
		render();
	}
}

void Viewer::update()
{
	for (auto& entity : entities)
	{
		entity->update();
	}
}

void Viewer::render()
{
	SDL_RenderClear(renderer);
	
	for (auto& entity : entities)
	{
		entity->render();
	}
	
	SDL_RenderPresent(renderer);
}

Color Viewer::getDrawColor()
{
	return drawColor;
}

void Viewer::setDrawColor(Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

