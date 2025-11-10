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

#include "player.h"

#include "sprites.h"
#include "common.h"
#include "bullet.h"
#include "explosion.h"
#include "world.h"
#include "sounds.h"
#include "game.h"

#include "raymath.h"

constexpr float BaseReloadTime = 0.5f;
constexpr float ShieldHitMaxLife = 0.35f;
constexpr float BreakingFriction = 0.025f;

Player::Player()
{
	Radius = Sprites::Frames[Sprites::ShipSprite].Frame.width * 0.5f;
}

void Player::Draw() const
{
	if (Alive)
	{
		if (Thrusting) // draw the thrust flame (below the ship so it doesn't clip over it)
		{
			float extension = Boost ? 25.0f : 3.0f;
			float sizeOffset = cosf((float)GetTime() * 20) * extension + (Radius * 1.2f);
			Vector2 offset = { 0, -sizeOffset * 0.5f };

			size_t sprite = Boost ? Sprites::TurboThustSprite : Sprites::ThrustSprite;

			Sprites::Draw(sprite, Position, Orientation, Vector2{ Radius * 0.45f, sizeOffset }, WHITE, offset);
		}

		// draw the ship
		Sprites::Draw(Sprites::ShipSprite, Position, Orientation);
	}
	
	// the sheild may need to be drawn after we die since that may be the hit that killed us
	if (ShieldHitLifetime > 0)
	{
		BeginBlendMode(BLEND_ADDITIVE);

		size_t sprite = Sprites::ShieldHitBase;

		float param = ShieldHitLifetime / ShieldHitMaxLife;

		float invParam = 1.0f - param;

		Color c = ColorAlpha(Sprites::ColorLerp(YELLOW, WHITE, param), param);

		float size = Radius * 3 + (invParam * Radius * 3);
		Sprites::Draw(Sprites::ShieldHitBase, Position, ShieldHitAngle, Vector2{ size,size }, c);

 		if (param < 0.3f)
 		   Sprites::Draw(Sprites::ShieldHitEnd, Position, ShieldHitAngle, Vector2{ size,size }, c);
		if (param < 0.6f)
			Sprites::Draw(Sprites::ShieldHitMid, Position, ShieldHitAngle, Vector2{ size,size }, c);
		
		EndBlendMode();
	}
}

void Player::Update()
{
	if (IsPaused())
		return;

	// decay the shield hit time (even if we are dead, so it animates)
	ShieldHitLifetime -= GetDeltaTime();

	if (!Alive)
		return;

	// add some shield power back
	Shield += GetDeltaTime() * ShieldRecharge;
	if (Shield > MaxShield)
		Shield = MaxShield;

	
	// gather our input states
	bool wantThrust = IsKeyDown(KEY_W) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT);
	bool wantBoost = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && wantThrust;
	bool wantShoot = IsKeyDown(KEY_SPACE) || IsMouseButtonDown(MOUSE_BUTTON_LEFT);
	bool wantBreak = IsKeyDown(KEY_S);

	if (IsGamepadAvailable(0))
	{
		wantThrust = wantThrust || GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_TRIGGER) > 0.125f;
		wantBoost = wantBoost || (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) && wantThrust);
		wantShoot = wantShoot || GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_TRIGGER) > 0.125f;
		wantBreak = wantBreak || IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT);
	}

	// handle rotation by mouse,keyboard, or gamepad
	if (Vector2LengthSqr(GetMouseDelta()) > 0)
	{
		auto mousePos = GetMousePosition();
		auto center = Vector2Scale(GetDisplaySize(), 0.5f);

		Vector2 mouseVec = Vector2Normalize(Vector2Subtract(mousePos, center));

		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			int j = 9;
		}

		if (Vector2LengthSqr(mouseVec) > 0)
		{
			Orientation = atan2f(mouseVec.y, mouseVec.x) * RAD2DEG + 90;
		}
	}
	else 
	{
		float rotation = 360 * GetDeltaTime();

		float input = 0;

		if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D))
		{
			if (IsKeyDown(KEY_A))
				input -= 1;
			if (IsKeyDown(KEY_D))
				input += 1;
		}
		else if (IsGamepadAvailable(0))
		{
			input = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
		}

		Orientation += input * rotation;
	}

	// ensure our angle is between -180 and +180
	while (Orientation > 180)
		Orientation -= 360;
	while (Orientation < -180)
		Orientation += 360;

	// boost if we can boost
	if (!wantBoost)
		Boost = false;
	else if (wantBoost && Power > MaxPower / 4)
		Boost = true;
	else if (Power <= 1)
		Boost = false;

	// adjust power levels
	if (Boost)
	{
		Power -= GetDeltaTime() * 400;
	}
	else if (Power < MaxPower)
	{
		Power += GetDeltaTime() * 20;
	}

	if (Power < 0)
		Power = 0;
	if (Power > MaxPower)
		Power = MaxPower;

	// decay the reload timer
	Reload -= GetDeltaTime() * ShotSpeedMultiplyer;
	
	// turn our angle into a vector so we can see what way we are going
	Vector2 shipVector = Vector2{ sinf(Orientation * DEG2RAD), -cosf(Orientation * DEG2RAD) };

	// see how much we could move this frame
	float speed = MaxThrust * GetDeltaTime();

	if (Boost)
		speed *= BoostMultiplyer;

	// add our desired thrust vector to our current vector, this is what gives us intertia
	if (wantThrust)
	{
		Velocity = Vector2Add(Velocity, Vector2Scale(shipVector, speed));
		Thrusting = true;
	}
	else
	{
		Thrusting = false;
	}
	Sounds::SetThrustState(Thrusting, Boost);

	// add in some friction to make the game a little more playable
	float frictionScale = 90;
	if (wantBreak)
		frictionScale *= BreakingFriction * MaxThrust;

	Vector2 normVel = Vector2Normalize(Velocity);
	Vector2 friction = Vector2Scale(normVel, -frictionScale * GetFrameTime());
	if (Vector2LengthSqr(friction) >= Vector2LengthSqr(Velocity))
		Velocity = { 0, 0 };
	else
		Velocity = Vector2Add(Velocity, friction);

	// don't let us move too fast
	float maxSpeed = Boost ? 5000.0f : 2000.0f;
	if (Vector2LengthSqr(Velocity) > maxSpeed * maxSpeed)
		Velocity = Vector2Scale(normVel, maxSpeed);

	// update based on our velocity
	Position = Vector2Add(Position, Vector2Scale(Velocity, GetDeltaTime()));

	// if we hit the walls of the world, bounce
	World::Instance->BounceBounds(*this);

	// if we can shoot, shoot. We do this after movement so the shot comes out in the right place
	if (wantShoot && Reload <= 0)
	{
		Reload = BaseReloadTime;

		Vector2 shotPos = Vector2Add(Position, Vector2Scale(shipVector, Radius * 1.0f));

		Vector2 shotVel = Vector2Add(Velocity, Vector2Scale(shipVector, 1500));
		Bullet::Create(shotPos, shotVel, Orientation);

		Sounds::PlaySoundEffect(Sounds::Shot);
	}
}

bool Player::Collide(const Entity& other)
{
	if (Entity::Collide(other))
	{
		Explosion::Create(other.Position, other.Radius);

		float damageFactor = 1.0f;
		if (Boost)
			damageFactor = 3.0f;

		Shield -= other.Radius * damageFactor;

		Sounds::PlaySoundEffect(Sounds::SheldHit);

		ShieldHitLifetime = ShieldHitMaxLife;

		Vector2 vectToHit = Vector2Subtract(other.Position, Position);
		ShieldHitAngle = atan2f(vectToHit.x, -vectToHit.y) * RAD2DEG;

		if (Shield < 0)
		{
			Explosion::Create(Position, 500);
			Alive = false;

			Sounds::PlaySoundEffect(Sounds::Destoryed);
		}

		return true;
	}

	return false;
}

void Player::Reset()
{
	Alive = true;
	Score = 0;
	Power = MaxPower;
	Shield = MaxShield;
	BoostMultiplyer = NominalBoostMultiplyer;
	MaxThrust = NominalThrust;
	ShieldRecharge = NominalShieldRecharge;
	Respawn();
}

void Player::Respawn()
{
	Boost = false;
	Thrusting = false;
	Sounds::SetThrustState(Thrusting, Boost);

	Velocity = Vector2{ 0,0 };
	Position = Vector2{ 0,0 };
	Orientation = 0;
	Reload = 0;
}

void Player::AddScore(int scoreDelta)
{
	if (!Alive)
		return;

	Score += scoreDelta;
}

bool Player::AcceptPressed()
{
	if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		return true;

	if (IsGamepadAvailable(0))
	{
		return IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
	}

	return false;
}
