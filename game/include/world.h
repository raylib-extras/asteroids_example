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

#pragma once

#include "entity.h"
#include "asteroid.h"
#include "bullet.h"
#include "explosion.h"
#include "player.h"
#include "powerup.h"

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
	std::vector<PowerUp> PowerUps;
	Player PlayerShip;

	bool IsLevelClear() const { return LevelClear; }

	size_t GetActiveAsteroidCount() const { return ActiveAsteroidCount; }

	bool BounceBounds(Entity& entity);

	bool Shake() const { return PlayerShip.Alive && PlayerShip.Boost; }

protected:
	bool LevelClear = false;
	size_t ActiveAsteroidCount = 0;
};
