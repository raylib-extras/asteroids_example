#pragma once

#include "entity.h"


class Player : public Entity
{
public:

	static constexpr float NominalShield = 1000;
	static constexpr float NominalPower = 1000;

	float Reload = 0;
	int Score = 0;
	bool Thrusting = false;
	bool Boost = false;

	float MaxShield = NominalShield;
	float Shield = MaxShield;

	float MaxPower = NominalPower;
	float Power = MaxPower;

	Player();
	void Draw() const override;
	void Update() override;

	bool Collide(const Entity& other) override;

	void Reset();
	void Respawn();

protected:
	float ShieldHitAngle = 0;
	float ShieldHitLifetime = -1;
};