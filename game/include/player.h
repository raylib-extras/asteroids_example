/**********************************************************************************************
*
*   Raylib Asteroids Example 'fasteroids++' * A simple RPG made using raylib
*
*    LICENSE: zlib/libpng
*
*   Copyright (c) 2023 Jeffery Myers
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

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