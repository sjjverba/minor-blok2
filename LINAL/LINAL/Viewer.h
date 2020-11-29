#pragma once
#include <memory>
#include <SDL_render.h>
#include <SDL_video.h>
#include <vector>
#include "Color.h"
#include "Entity.h"
#include "Events.h"
#include "Input.h"

class Viewer
{
public:
	Viewer();
	void startLoop();
	void update();
	void render();

	static Color getDrawColor();
	static void setDrawColor(Color color);

	static SDL_Window* window;
	static event<> onWindowResize;
	static SDL_Renderer* renderer;
	static bool running;
	static int screenWidth;
	static int screenHeight;
private:
	Input input;
	static  Color drawColor;
	std::vector<std::unique_ptr<Entity>> entities;
};



