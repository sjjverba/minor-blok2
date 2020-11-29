#pragma once
#include <xstring>

#include "Point.h"
#include "Color.h"
#include "Entity.h"

class Line : public Entity
{
public:
	Line(Point from, Point to, int radius, Color color) : from(from) , to(to) , radius(radius) , color(color) { }
	Point from;
	Point to;
	int radius;
	Color color;
	void update() override;
	void render() override;
	void addThickness(std::string& direction, int distance, bool isUpperPart);
};

