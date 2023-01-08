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

#include "raylib.h"
#include "raymath.h"

// base entity class that contains common data used by all entity types
class Entity
{
public:
	virtual ~Entity() = default;

	bool Alive = false;	

	Vector2 Position = { 0 };
	float Orientation = 0;

	Vector2 Velocity = { 0 };
	float RotationalVelocity = 0;

	float Radius = 0;

	virtual void Update();

	virtual bool Collide(const Entity& other);

	inline virtual void Draw() const {}
};