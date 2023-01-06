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

	PlayerShip.Respawn();

	int count = 40 + (20 * level);
	for (int i = 0; i < count; i++)
	{
		Vector2 pos = GetRandomVector2(Bounds);

		while (Vector2DistanceSqr(PlayerShip.Position, pos) < safeRad* safeRad)
			pos = GetRandomVector2(Bounds);

		Vector2 Velocity = { GetRandomValueF(-50,50) ,GetRandomValueF(-50,50) };

		Asteroid::Create(GetRandomValueF(40, 200), pos, Velocity);
	}
}

void World::Update()
{
	size_t asteroidCount = 0;

	for (auto& asteroid : Asteroids)
	{
		asteroid.Update();

		if (PlayerShip.Collide(asteroid))
			asteroid.Alive = false;

		if (asteroid.Alive)
			asteroidCount++;
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

	PlayerShip.Update();
	
	if (asteroidCount == 0)
	{
		LevelClear = true;
	}
}

void World::Draw() const
{
	DrawRectangleLinesEx(Bounds, 40, SKYBLUE);

	for (auto& asteroid : Asteroids)
		asteroid.Draw();

	BeginBlendMode(BLEND_ADDITIVE);

	for (auto& bullet : Bullets)
		bullet.Draw();

	for (auto& exlosion : Explosions)
		exlosion.Draw();
	EndBlendMode();

	PlayerShip.Draw();
}