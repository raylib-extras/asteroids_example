#include "player.h"

#include "sprites.h"
#include "common.h"
#include "bullet.h"

Player::Player()
{
	Radius = Sprites::ShipSprite.width * 0.5f;
}

void Player::Draw() const
{
	if (Thrusting)
	{
		Vector2 center = Vector2{ Sprites::ShipSprite.width / 2.0f, -Sprites::ShipSprite.height / 4.0f };
		Rectangle dest = { Position.x, Position.y, Sprites::ShotSprite.width,Sprites::ShotSprite.height + cosf((float)GetTime() * 30) * 20 };
		DrawTexturePro(Sprites::SpriteSheet, Turbo ? Sprites::TurboThustSprite : Sprites::ThrustSprite, dest, center, Orientation, WHITE);
	}

	Sprites::Draw(Sprites::ShipSprite, Position, Orientation);
}

void Player::Update()
{
	Thrusting = false;
	Turbo = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && IsKeyDown(KEY_W);

	Reload -= GetDeltaTime();

	float rotation = 270 * GetDeltaTime();

	if (IsKeyDown(KEY_A))
		Orientation -= rotation;
	if (IsKeyDown(KEY_D))
		Orientation += rotation;

	Vector2 shipVector = Vector2{ sinf(Orientation * DEG2RAD), -cosf(Orientation * DEG2RAD) };

	float speed = 30 + GetDeltaTime();

	if (Turbo)
		speed *= 3;

	if (IsKeyDown(KEY_W))
	{
		Velocity = Vector2Add(Velocity, Vector2Scale(shipVector, speed));
		Thrusting = true;
	}

	if (IsKeyDown(KEY_S))
		Velocity = Vector2Add(Velocity, Vector2Scale(shipVector, -speed * 0.25f));

	Vector2 normVel = Vector2Normalize(Velocity);
	Vector2 friction = Vector2Scale(normVel, -90.0f * GetFrameTime());
	if (Vector2LengthSqr(friction) >= Vector2LengthSqr(Velocity))
		Velocity = { 0, 0 };
	else
		Velocity = Vector2Add(Velocity, friction);

	float maxSpeed = Turbo ? 5000.0f : 2000.0f;
	if (Vector2LengthSqr(Velocity) > maxSpeed * maxSpeed)
		Velocity = Vector2Scale(normVel, maxSpeed);

	Position = Vector2Add(Position, Vector2Scale(Velocity, GetDeltaTime()));

	// normalize angle
	while (Orientation > 180)
		Orientation -= 360;
	while (Orientation < -180)
		Orientation += 360;

	if (IsKeyDown(KEY_SPACE) && Reload <= 0)
	{
		// fire
		Reload = 0.125f;

		Vector2 shotPos = Vector2Add(Position, Vector2Scale(shipVector, Radius * 1.0f));

		Vector2 shotVel = Vector2Add(Velocity, Vector2Scale(shipVector, 1500));
		Bullet::Create(shotPos, shotVel);
	}
}

Player PlayerShip;