#pragma once
#include <vector>

#include "Entity.h"
#include "Line.h"

class CoordinateSystem : public Entity
{
public:
	CoordinateSystem(int rows , int columns);
	void reDraw();
	void update() override;
	void render() override;
private:
	std::vector<Line> lines;
};

