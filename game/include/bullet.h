#pragma once

#include "entity.h"

class Bullet : public Entity
{
public:
	Color Tint = WHITE;
	float Lifetime = 0;

	Bullet();

	void Draw() const override;

	void Update() override;

	static void Create(const Vector2& pos, const Vector2& velocity, float orientation);
};