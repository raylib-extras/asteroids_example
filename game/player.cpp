#include "player.h"

#include "sprites.h"
#include "common.h"
#include "bullet.h"
#include "explosion.h"
#include "world.h"
#include "sounds.h"
#include "game.h"

constexpr float BaseReloadTime = 0.5f;
constexpr float ShieldHitMaxLife = 0.35f;
constexpr float BreakingFriction = 0.025f;

Player::Player()
{
	Radius = Sprites::Frames[Sprites::ShipSprite].Frame.width * 0.5f;
}

void Player::Draw() const
{
	if (Thrusting && Alive)
	{
		float extension = Boost ? 25 : 3;
		float sizeOffset = cosf((float)GetTime() * 20) * extension + (Radius*1.2f);
		Vector2 offset = { 0, -sizeOffset * 0.5f };

		size_t sprite = Boost ? Sprites::TurboThustSprite : Sprites::ThrustSprite;

		Sprites::Draw(sprite, Position, Orientation, Vector2{Radius*0.45f, sizeOffset}, WHITE, offset);
	}

	if (Alive)
		Sprites::Draw(Sprites::ShipSprite, Position, Orientation);

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

	ShieldHitLifetime -= GetDeltaTime();

	if (!Alive)
		return;

	Shield += GetDeltaTime() * ShieldRecharge;
	if (Shield > MaxShield)
		Shield = MaxShield;

	bool wantThrust = IsKeyDown(KEY_W) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT);
	Thrusting = false;

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

	if (Vector2LengthSqr(GetMouseDelta()) > 0)
	{
		Vector2 mouseVec = Vector2Normalize(Vector2Subtract(GetMousePosition(), Vector2Scale(GetDisplaySize(), 0.5f)));

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

	if (!wantBoost)
		Boost = false;
	else if (wantBoost && Power > MaxPower / 4)
		Boost = true;
	else if (Power <= 1)
		Boost = false;

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

	Reload -= GetDeltaTime() * ShotSpeedMultiplyer;
	
	Vector2 shipVector = Vector2{ sinf(Orientation * DEG2RAD), -cosf(Orientation * DEG2RAD) };

	float speed = MaxThrust * GetDeltaTime();

	if (Boost)
		speed *= BoostMultiplyer;

	if (wantThrust)
	{
		Velocity = Vector2Add(Velocity, Vector2Scale(shipVector, speed));
		Thrusting = true;
	}

	float frictionScale = 90;
	if (wantBreak)
		frictionScale *= BreakingFriction * MaxThrust;

	Vector2 normVel = Vector2Normalize(Velocity);
	Vector2 friction = Vector2Scale(normVel, -frictionScale * GetFrameTime());
	if (Vector2LengthSqr(friction) >= Vector2LengthSqr(Velocity))
		Velocity = { 0, 0 };
	else
		Velocity = Vector2Add(Velocity, friction);

	float maxSpeed = Boost ? 5000.0f : 2000.0f;
	if (Vector2LengthSqr(Velocity) > maxSpeed * maxSpeed)
		Velocity = Vector2Scale(normVel, maxSpeed);

	Position = Vector2Add(Position, Vector2Scale(Velocity, GetDeltaTime()));

	World::Instance->BounceBounds(*this);

	// normalize angle
	while (Orientation > 180)
		Orientation -= 360;
	while (Orientation < -180)
		Orientation += 360;

	if (wantShoot && Reload <= 0)
	{
		// fire
		Reload = BaseReloadTime;

		Vector2 shotPos = Vector2Add(Position, Vector2Scale(shipVector, Radius * 1.0f));

		Vector2 shotVel = Vector2Add(Velocity, Vector2Scale(shipVector, 1500));
		Bullet::Create(shotPos, shotVel, Orientation);

		Sounds::PlaySoundEffect(Sounds::Shot);
	}

	Sounds::SetThrustState(Thrusting, Boost);
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
