#pragma once

#include "entity.h"

class PowerUp : public Entity
{
public:
	enum class PowerType
	{
		Shot = 0,
		Shield,
		Boost
	};

	PowerType Type = PowerType::Shot;

	void Draw() const override;

	bool Collide(const Entity& other) override;

	static void Create(const Vector2& pos);
};
