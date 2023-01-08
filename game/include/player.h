#pragma once

#include "entity.h"

class Player : public Entity
{
public:
	static constexpr float NominalShield = 1000;
	static constexpr float NominalShieldRecharge = 2;
	static constexpr float NominalPower = 1000;
	static constexpr float NominalThrust = 400;
	static constexpr float NominalBoostMultiplyer = 3;

	float Reload = 0;

	bool Thrusting = false;
	bool Boost = false;

	float ShotSpeedMultiplyer = 1;

	float MaxShield = NominalShield;
	float Shield = MaxShield;

	float MaxPower = NominalPower;
	float Power = MaxPower;

	float MaxThrust = NominalThrust;
	float BoostMultiplyer = NominalBoostMultiplyer;

	float ShieldRecharge = NominalShieldRecharge;

	Player();
	void Draw() const override;
	void Update() override;

	bool Collide(const Entity& other) override;

	void Reset();
	void Respawn();

	void AddScore(int scoreDelta);
	inline int GetScore() const { return Score; }

	bool AcceptPressed();

protected:
	float ShieldHitAngle = 0;
	float ShieldHitLifetime = -1;

	int Score = 0;
};