#include "powerup.h"
#include "world.h"
#include "common.h"
#include "sprites.h"
#include "sounds.h"

constexpr float PowerupRadius = 55;

void PowerUp::Update()
{
	Entity::Update();
	World::Instance->BounceBounds(*this);
}

void PowerUp::Draw() const
{
	if (!Alive)
		return;

	size_t sprite = Sprites::BoostPowerup;
	switch (Type)
	{
		case PowerUp::PowerType::Shot:
			sprite = Sprites::ShotPowerup;
			break;
		case PowerUp::PowerType::Shield:
			sprite = Sprites::ShieldPowerup;
			break;
	}

	Sprites::Draw(sprite, Position, Orientation);
}

bool PowerUp::Collide(const Entity& other)
{
	if (Entity::Collide(other))
	{
		Sounds::PlaySoundEffect(Sounds::Upgrade);

		Player& player = World::Instance->PlayerShip;
		switch (Type)
		{
			case PowerUp::PowerType::Shot:
				player.ShotSpeedMultiplyer += 0.10f;
				break;

			case PowerUp::PowerType::Shield:
				player.Shield += World::Instance->PlayerShip.MaxShield / 8.0f;
				player.ShieldRecharge += player.NominalShieldRecharge;

				break;

			case PowerUp::PowerType::Boost:
				player.Power += World::Instance->PlayerShip.MaxPower / 4.0f;
				player.BoostMultiplyer += 0.1f;
				player.MaxThrust += player.NominalThrust * 0.25f;
				break;
		}

		Alive = false;
		return true;
	}
	return false;
}

void PowerUp::Create(const Vector2& pos)
{
	PowerUp* slot = nullptr;
	// find an empty asteroid

	for (auto& powerup : World::Instance->PowerUps)
	{
		if (!powerup.Alive)
		{
			slot = &powerup;
			break;
		}
	}

	if (!slot)
	{
		World::Instance->PowerUps.emplace_back(PowerUp());
		slot = &(World::Instance->PowerUps.back());
	}

	slot->Alive = true;
	slot->Radius = PowerupRadius;
	slot->Position = pos;
	slot->Orientation = (float)GetRandomValue(-180, 180);

	Vector2 velocity = { GetRandomValueF(-50,50) ,GetRandomValueF(-50,50) };

	slot->Velocity = velocity;
	slot->RotationalVelocity = (float)GetRandomValue(-360, 360);

	slot->Type = (PowerType)GetRandomValue(0, 2);
}
