#include "world.h"
#include "common.h"

World* World::Instance = nullptr;

void World::Reset(int level)
{
	Asteroids.clear();
	Bullets.clear();
	Explosions.clear();

	int count = 40 + (20 * level);

	for (int i = 0; i < count; i++)
	{
		Vector2 pos = GetRandomVector2(Bounds);
		Vector2 Velocity = { GetRandomValueF(-50,50) ,GetRandomValueF(-50,50) };

		Asteroid::Create(GetRandomValueF(40, 200), pos, Velocity);
	}
}

void World::Update()
{
	for (auto& asteroid : Asteroids)
		asteroid.Update();

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
}

void World::Draw()
{

	DrawRectangleLinesEx(Bounds, 40, SKYBLUE);

	for (auto& asteroid : Asteroids)
		asteroid.Draw();

	for (auto& bullet : Bullets)
		bullet.Draw();

	for (auto& exlosion : Explosions)
		exlosion.Draw();

	PlayerShip.Draw();
}