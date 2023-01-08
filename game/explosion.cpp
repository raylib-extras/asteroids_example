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

#include "explosion.h"
#include "sprites.h"
#include "common.h"
#include "world.h"

constexpr float MaxLifetime = 0.5f;

void Explosion::Draw() const
{
	if (!Alive)
		return;

	float param = Lifetime / MaxLifetime;

	Color c = ColorAlpha(Sprites::ColorLerp(YELLOW, RED , param), param);
	
	for (const auto& particle : Particles)
	{
		Sprites::Draw(Sprites::ParticleSprite, Vector2Add(Position, particle.Position), Orientation, -1, c);
	}
}

void Explosion::Update()
{
	if (!Alive)
		return;

	Lifetime -= GetDeltaTime();

	if (Lifetime < 0)
		Alive = false;

	Entity::Update();

	for (auto& particle : Particles)
		particle.Update();
}

void Explosion::Create(const Vector2& pos, float size)
{
	Explosion* slot = nullptr;
	// find an empty shot

	for (auto& explosion : World::Instance->Explosions)
	{
		if (!explosion.Alive)
		{
			slot = &explosion;
			break;
		}
	}

	if (!slot)
	{
		World::Instance->Explosions.emplace_back(Explosion());
		slot = &(World::Instance->Explosions.back());
	}

	slot->Alive = true;
	slot->Position = pos;
	slot->Velocity = Vector2{ 0,0 };
	slot->Orientation = 0;
	slot->RotationalVelocity = 0;
	slot->Lifetime = MaxLifetime;

	slot->Particles.clear();

	int particles = GetRandomValue(3, 8);

	int add = int(size / 3.0f);
	particles += add;

	for (int i = 0; i < particles; i++)
	{
		Entity particle;
		particle.Alive = true;
		particle.Orientation = (float)GetRandomValue(0, 180);
		particle.RotationalVelocity = (float)GetRandomValue(180, 720);

		float randomAngle = (float)GetRandomValue(-180, 180) * DEG2RAD;
		float randomSpeed = (float)GetRandomValue(20, 500);

		particle.Velocity = Vector2{ cosf(randomAngle) * randomSpeed, sinf(randomAngle) * randomSpeed };

		slot->Particles.emplace_back(std::move(particle));
	}
}

