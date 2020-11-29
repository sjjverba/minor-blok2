#include "Line.h"

#include <iostream>
#include <SDL_render.h>
#include <string>

#include "Viewer.h"

void Line::update()
{
}

void Line::render()
{
	if (radius == 0)
		return;
	
	Color color = Viewer::getDrawColor();
	Viewer::setDrawColor(this->color);

	const auto xDiff = to.x - from.x;
	const auto yDiff = to.y - from.y;

	const auto total = std::abs(xDiff) + std::abs(yDiff);
	const auto xPercentage = std::abs(xDiff) / total;

	std::string direction;
	
	if (xPercentage > 0.5)
	{
		direction = "horizontal";
	}
	else
	{
		direction = "vertical";
	}


	for (int i = 0; i < radius / 2; i++)
	{
		addThickness(direction, i + 1, false);
	}

	for (int i = radius / 2 + 1; i < radius; i++)
	{
		auto index = i - radius / 2;
		addThickness(direction, index, true);
	}

	SDL_RenderDrawLine(Viewer::renderer, from.x, from.y, to.x, to.y);	
	Viewer::setDrawColor(color);
}

void Line::addThickness(std::string& direction, int distance , bool isUpperPart)
{
	Point newFrom {from.x , from.y};
	Point newTo {to.x, to.y};
	int increment = isUpperPart ? -1 : 1;
	
	if (direction == "horizontal")
	{
		newFrom.y = from.y + increment * distance;
		newTo.y = to.y + increment * distance;
	}
	else if(direction == "vertical")
	{
		newFrom.x = from.x + increment * distance;
		newTo.x = to.x + increment * distance;
	}
	else if (direction == "diagonal")
	{
		newFrom.x = from.x + increment * distance;
		newTo.x = to.x + increment * distance;

		newFrom.y = from.y - increment * distance;
		newTo.y = to.y - increment * distance;
	}
	
	SDL_RenderDrawLine(Viewer::renderer, newFrom.x, newFrom.y, newTo.x, newTo.y);
}
