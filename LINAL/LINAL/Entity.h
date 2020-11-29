#pragma once
class Entity
{
public:
	virtual ~Entity() = default;
	virtual void render() = 0;
	virtual void update() = 0;
};

