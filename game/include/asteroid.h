#pragma once

#include "entity.h"

class Asteroid : public Entity
{
public:
	Rectangle Sprite = { 0 };

	Color Tint = WHITE;

	void Draw() const override;

	bool Collide(const Entity& other) override;

	static void Create(float radius, const Vector2& pos, const Vector2& velocity);
};
