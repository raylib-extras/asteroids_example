#pragma once

#include "entity.h"
#include "asteroid.h"
#include "bullet.h"
#include "explosion.h"
#include "player.h"

#include <vector>

class World
{
public:
	static World* Instance;

	World() { Instance = this; }
	virtual ~World() { if (Instance == this) Instance = nullptr; }

	Rectangle Bounds = { -6000, -6000, 12000, 12000 };

	void Reset(int level = 1);

	void Update();

	void Draw(Rectangle& screenInWorld) const;

	std::vector<Asteroid> Asteroids;
	std::vector<Bullet> Bullets;
	std::vector<Explosion> Explosions;
	Player PlayerShip;

	bool IsLevelClear() const { return LevelClear; }

	bool BounceBounds(Entity& entity);

protected:
	bool LevelClear = false;
};
