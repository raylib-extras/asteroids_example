#include "bullet.h"
#include "sprites.h"
#include "common.h"
#include "world.h"

Bullet::Bullet()
{
	Radius = 10;
}

void Bullet::Draw() const
{
	if (!Alive)
		return;

	Sprites::Draw(Sprites::ShotSprite, Position, Orientation, -1, Tint);

}

void Bullet::Update()
{
	if (!Alive)
		return;

	Lifetime -= GetDeltaTime();

	if (Lifetime < 0)
		Alive = false;

	Entity::Update();
}

void Bullet::Create(const Vector2& pos, const Vector2& velocity)
{
	Bullet* slot = nullptr;
	// find an empty shot

	for (auto& bullet : World::Instance->Bullets)
	{
		if (!bullet.Alive)
		{
			slot = &bullet;
			break;
		}
	}

	if (!slot)
	{
		World::Instance->Bullets.emplace_back(Bullet());
		slot = &(World::Instance->Bullets.back());
	}

	slot->Alive = true;
	slot->Position = pos;
	slot->Velocity = velocity;
	slot->Orientation = 0;
	slot->RotationalVelocity = (float)GetRandomValue(20, 1000);
	slot->Lifetime = 5;
	slot->Tint = RED;
}