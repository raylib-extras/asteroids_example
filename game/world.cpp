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

		float velLimits = 50 + (25 * level);
		Vector2 Velocity = { GetRandomValueF(-velLimits,velLimits) ,GetRandomValueF(-velLimits,velLimits) };

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

void World::Draw(Rectangle& screenInWorld) const
{
	DrawRectangleLinesEx(Bounds, 40, SKYBLUE);

	float viewRadSq = powf(screenInWorld.width * 0.65f, 2) + powf(screenInWorld.height * 0.65f, 2);

	for (auto& asteroid : Asteroids)
	{
		// only draw asteroids that are near our viewable window
		if (asteroid.Alive && Vector2DistanceSqr(PlayerShip.Position,asteroid.Position) < viewRadSq + (powf(asteroid.Radius,2)))
			asteroid.Draw();
	}

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
