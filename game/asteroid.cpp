#include "asteroid.h"

#include "sprites.h"
#include "common.h"
#include "sounds.h"
#include "world.h"

void Asteroid::Update()
{
	Entity::Update();
	if (!Alive)
		return;
	World::Instance->BounceBounds(*this);
}

void Asteroid::Draw() const
{
	if (!Alive)
		return;

	Sprites::Draw(Sprite, Position, Orientation, Radius, Tint);
}

bool Asteroid::Collide(const Entity& other)
{
	bool hit = CheckCollisionCircles(Position, Radius, other.Position, other.Radius);

	if (hit)
	{
		Sounds::PlaySoundEffect(Sounds::AsteroidHit);

		float split1Radius = GetRandomValue(35, 75) / 100.0f;
		float split2Radius = 1.0f - split1Radius;

		float rad = Radius - 2; // remove a little bit of mass from the shot

		split1Radius *= rad;
		split2Radius *= rad;

		float splitAngle = (float)GetRandomValue(-180, 180);
		Vector2 newVector = { -sinf(splitAngle * DEG2RAD), cosf(splitAngle * DEG2RAD) };

		float speed = Vector2Length(Velocity);

		// make two new asteroids
		Vector2 newPos = Vector2Add(Position, Vector2Scale(newVector, split1Radius * 0.25f));
		Create(split1Radius, newPos, Vector2Scale(newVector, GetRandomValueF(speed + 20, speed + 80)));

		newVector = Vector2Scale(newVector, -1);
		newPos = Vector2Add(Position, Vector2Scale(newVector, split2Radius * 0.25f));
		Create(split2Radius, newPos, Vector2Scale(newVector, GetRandomValueF(speed + 20, speed + 80)));

		World::Instance->PlayerShip.Score += 1;

		Explosion::Create(Position, Radius);

		Alive = false;
	}
	return hit;
}

void Asteroid::Create(float radius, const Vector2& pos, const Vector2& velocity)
{
	// don't make an asteroid that is too small

	if (radius < 40)
	{
		World::Instance->PlayerShip.Score += int(radius + 0.5f);
		return;
	}
	
	Asteroid* slot = nullptr;
	// find an empty asteroid

	for (auto& asteroid : World::Instance->Asteroids)
	{
		if (!asteroid.Alive)
		{
			slot = &asteroid;
			break;
		}
	}

	if (!slot)
	{
		World::Instance->Asteroids.emplace_back(Asteroid());
		slot = &(World::Instance->Asteroids.back());
	}

	slot->Alive = true;
	slot->Radius = radius;
	slot->Position = pos;
	slot->Orientation = (float)GetRandomValue(-180, 180);
	slot->Velocity = velocity;
	slot->RotationalVelocity = (float)GetRandomValue(-90, 90);

	slot->Tint = WHITE;// Sprites::ColorLerp(DARKBROWN, BEIGE, GetRandomValueF(0, 1));

	slot->Sprite = Sprites::GetRandomAsteroid();
}