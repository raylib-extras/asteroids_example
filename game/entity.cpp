#include "entity.h"

#include "common.h"

void Entity::Update()
{
	if (!Alive)
		return;

	// move
	Position = Vector2Add(Position, Vector2Scale(Velocity, GetDeltaTime()));

	// rotate
	Orientation += RotationalVelocity * GetDeltaTime();

	// normalize angle
	while (Orientation > 180)
		Orientation -= 360;
	while (Orientation < -180)
		Orientation += 360;
}

bool Entity::Collide(const Entity& other)
{
	return Alive && CheckCollisionCircles(Position, Radius, other.Position, other.Radius);
}

