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
#include <stdlib.h>

namespace Sounds
{
	extern size_t AsteroidHit;
	extern size_t Destoryed;
	extern size_t SheldHit;
	extern size_t Shot;
	extern size_t Upgrade;

	extern size_t GameOver;
	extern size_t Begin;

	void Init();
	void Shutdown();
	void Update();

	void SetThrustState(bool thrusting, bool boosting);

	void StartBGM();
	void StopBGM();

	void PlaySoundEffect(size_t effect);
}