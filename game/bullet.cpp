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

#include "bullet.h"
#include "sprites.h"
#include "common.h"
#include "world.h"

constexpr float MaxLife = 5;

Bullet::Bullet()
{
	Radius = 10;
}

void Bullet::Draw() const
{
	if (!Alive)
		return;

	Sprites::Draw(Sprites::ShotSprite, Position, Orientation, -1, ColorAlpha(Tint, Lifetime/ MaxLife));
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

void Bullet::Create(const Vector2& pos, const Vector2& velocity, float orientation)
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
	slot->Orientation = orientation;
//	slot->RotationalVelocity = (float)GetRandomValue(20, 1000);
	slot->Lifetime = MaxLife;
	slot->Tint = RED;
}