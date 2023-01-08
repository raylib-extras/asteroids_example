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

#include "world.h"
#include "common.h"

World* World::Instance = nullptr;

void World::Reset(int level)
{
	float safeRad = 200;

	LevelClear = false;
	Asteroids.clear();
	Bullets.clear();
	Explosions.clear();
	PowerUps.clear();

	PlayerShip.Respawn();

	Bounds.x = Bounds.y = -3000 - (level * 500.0f);
	Bounds.width = Bounds.height = -(Bounds.x * 2);

	int count = 50 + (40 * level);
	for (int i = 0; i < count; i++)
	{
		Vector2 pos = GetRandomVector2(Bounds);

		while (Vector2DistanceSqr(PlayerShip.Position, pos) < safeRad* safeRad)
			pos = GetRandomVector2(Bounds);

		float velLimits = 50 + (35.0f * level);
		Vector2 Velocity = { GetRandomValueF(-velLimits,velLimits) ,GetRandomValueF(-velLimits,velLimits) };

		Asteroid::Create(GetRandomValueF(40, 200), pos, Velocity);
	}
}

void World::Update()
{
	ActiveAsteroidCount = 0;

	for (auto& asteroid : Asteroids)
	{
		asteroid.Update();

		if (PlayerShip.Collide(asteroid))
			asteroid.Alive = false;

		if (asteroid.Alive)
			ActiveAsteroidCount++;
	}

	for (auto& bullet : Bullets)
	{
		bullet.Update();

		if (!bullet.Alive)
			continue;

		for (auto& asteroid : Asteroids)
		{
			if (!asteroid.Alive)
				continue;

			if (asteroid.Collide(bullet))
			{
				bullet.Alive = false;
				break;
			}
		}
	}

	for (auto& exlosion : Explosions)
		exlosion.Update();

	for (auto& powerup : PowerUps)
	{
		powerup.Update();
		if (powerup.Collide(PlayerShip))
			powerup.Alive = false;
	}

	PlayerShip.Update();
	
	if (ActiveAsteroidCount == 0)
	{
		LevelClear = true;
	}
}

void World::Draw(Rectangle& screenInWorld) const
{
	float thickness = 30 + sinf((float)GetTime() * 10) * 10;
	Rectangle bounds = { Bounds.x - thickness, Bounds.y - thickness, Bounds.width + thickness * 2, Bounds.height + thickness + 2 };
	DrawRectangleLinesEx(bounds, thickness, BLUE);

	float innerThickness = thickness / 2;
	float innerThickoffset = thickness - (thickness - innerThickness)/2;
	bounds = Rectangle{ Bounds.x - innerThickoffset, Bounds.y - innerThickoffset, Bounds.width + innerThickoffset * 2, Bounds.height + innerThickoffset + 2 };
	DrawRectangleLinesEx(bounds, innerThickness, SKYBLUE);

	float viewRadSq = powf(screenInWorld.width * 0.65f, 2) + powf(screenInWorld.height * 0.65f, 2);

	for (auto& asteroid : Asteroids)
	{
		// only draw asteroids that are near our viewable window
		if (asteroid.Alive && Vector2DistanceSqr(PlayerShip.Position,asteroid.Position) < viewRadSq + (powf(asteroid.Radius,2)))
			asteroid.Draw();
	}

	for (auto& powerups : PowerUps)
		powerups.Draw();

	BeginBlendMode(BLEND_ADDITIVE);

	for (auto& bullet : Bullets)
		bullet.Draw();

	for (auto& exlosion : Explosions)
		exlosion.Draw();
	EndBlendMode();

	PlayerShip.Draw();
}

bool World::BounceBounds(Entity& entity)
{
	bool hit = false;

	float left = Bounds.x + entity.Radius;
	float right = Bounds.x + Bounds.width - entity.Radius;
	if (entity.Position.x < left)
	{
		entity.Position.x = left;
		entity.Velocity.x *= -1;
		hit = true;
	}
	else if (entity.Position.x > right)
	{
		entity.Position.x = right;
		entity.Velocity.x *= -1;
		hit = true;
	}

	float top = Bounds.y + entity.Radius;
	float bottom = Bounds.y + Bounds.height - entity.Radius;
	if (entity.Position.y < top)
	{
		entity.Position.y = top;
		entity.Velocity.y *= -1;
		hit = true;
	}else if (entity.Position.y > bottom)
	{
		entity.Position.y = bottom;
		entity.Velocity.y *= -1;
		hit = true;
	}

	return hit;
}
