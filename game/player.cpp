#include "player.h"

#include "sprites.h"
#include "common.h"
#include "bullet.h"
#include "explosion.h"
#include "world.h"
#include "sounds.h"

constexpr float BaseReloadTime = 0.5f;
constexpr float ShieldHitMaxLife = 0.35f;

Player::Player()
{
	Radius = Sprites::Frames[Sprites::ShipSprite].Frame.width * 0.5f;
}

void Player::Draw() const
{
	if (Thrusting && Alive)
	{
		float sizeOffset = cosf((float)GetTime() * 20) * 3 + (Radius*1.2f);
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
	ShieldHitLifetime -= GetDeltaTime();

	if (!Alive)
		return;

	Shield += GetDeltaTime() * 2;
	if (Shield > 1000)
		Shield = 1000;

	// TODO, game pad support

	bool wantThrust = IsKeyDown(KEY_W) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT);
	Thrusting = false;

	bool wantBoost = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && wantThrust;

	if (!wantBoost)
		Boost = false;
	else if (wantBoost && Power > 250)
		Boost = true;
	else if (Power <= 1)
		Boost = false;

	if (Boost)
	{
		Power -= GetDeltaTime() * 400;
	}
	else if (Power < 1000)
	{
		Power += GetDeltaTime() * 20;
	}

	if (Power < 0)
		Power = 0;
	if (Power > 1000)
		Power = 1000;

	Reload -= GetDeltaTime() * ShotSpeedMultiplyer;

	float rotation = 270 * GetDeltaTime();

	Vector2 mouseVec = Vector2Normalize(Vector2Subtract(GetMousePosition(), Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }));

	if (Vector2LengthSqr(mouseVec) > 0)
	{
		Orientation = atan2f(mouseVec.y, mouseVec.x) * RAD2DEG + 90;
	}

	Vector2 shipVector = Vector2{ sinf(Orientation * DEG2RAD), -cosf(Orientation * DEG2RAD) };

	float speed = 30 + GetDeltaTime();

	if (Boost)
		speed *= 3;

	if (wantThrust)
	{
		Velocity = Vector2Add(Velocity, Vector2Scale(shipVector, speed));
		Thrusting = true;
	}

	float frictionScale = 1;
	if (IsKeyDown(KEY_S))
		frictionScale = 30;

	Vector2 normVel = Vector2Normalize(Velocity);
	Vector2 friction = Vector2Scale(normVel, -90.0f * frictionScale * GetFrameTime());
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

	bool wantShoot = IsKeyDown(KEY_SPACE) || IsMouseButtonDown(MOUSE_BUTTON_LEFT);

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

		Shield -= other.Radius / 1.0f;

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
