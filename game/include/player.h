#pragma once

#include "entity.h"

class Player : public Entity
{
public:
	float Reload = 0;
	int Score = 0;
	bool Thrusting = false;
	bool Turbo = false;

	Player();
	void Draw() const override;
	void Update() override;
};