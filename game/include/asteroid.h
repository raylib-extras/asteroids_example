#pragma once

#include "entity.h"

class Asteroid : public Entity
{
public:
	size_t Sprite = 0;

	Color Tint = WHITE;

	void Update() override;
	void Draw() const override;

	bool Collide(const Entity& other) override;

	static void Create(float radius, const Vector2& pos, const Vector2& velocity);
};
