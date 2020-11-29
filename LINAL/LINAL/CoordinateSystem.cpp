#include "CoordinateSystem.h"

#include <iostream>

#include "Line.h"
#include "Viewer.h"
#include "Events.h"

CoordinateSystem::CoordinateSystem(int rows, int columns)
{
	Viewer::onWindowResize += [=]() {reDraw(); };	
}

void CoordinateSystem::reDraw()
{
	std::cout << "resize" << std::endl;

	lines.clear();

	auto line = std::make_unique<Line>(
		Point{ 50,50 },
		Point{ 350,250 },
		8,
		Color{ 125,255,255,255 }
	);

	lines.emplace_back(std::move(line));
}

void CoordinateSystem::update()
{
	
}

void CoordinateSystem::render()
{
	
}
