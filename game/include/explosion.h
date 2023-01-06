#pragma once

#include "entity.h"
#include <vector>

class Explosion : public Entity
{
public:
	void Draw() const override;

	void Update() override;

	static void Create(const Vector2& pos, float size);

protected:
	Color Tint = WHITE;
	float Lifetime = 0;

	std::vector<Entity> Particles;
};