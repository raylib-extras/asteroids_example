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

#include <vector>
#include <stdlib.h>

namespace Sprites
{
	class SpriteFrame
	{
	public:
		size_t Sheet = 0;
		Rectangle Frame;

		Rectangle Borders = { 0 };

		SpriteFrame() {}
		SpriteFrame(size_t sheet, float x, float y, float w, float h)
			: Sheet(sheet)
			, Frame{ x,y,w,h }

		{
		}
	};

	extern std::vector<SpriteFrame> Frames;
	extern std::vector<Texture> SpriteSheets;

	extern size_t ShipSprite;
	extern size_t ShotSprite;
	extern size_t ThrustSprite;
	extern size_t TurboThustSprite;
	extern size_t ParticleSprite;
	extern size_t MiniMapSprite;
	extern size_t ShieldIcon;
	extern size_t BoostIcon;
	extern size_t ShieldBar;
	extern size_t BoostBar;
	extern size_t ShieldProgress;
	extern size_t BoostProgress;

	extern size_t ShieldHitBase;
	extern size_t ShieldHitMid;
	extern size_t ShieldHitEnd;

	extern size_t ShieldPowerup;
	extern size_t BoostPowerup;
	extern size_t ShotPowerup;

	extern size_t Cursor;

	extern size_t NavArrow;

	void Init();
	void Shutdown();

	void Draw(size_t frame, const Vector2& pos, float rotation = 0, float size = -1, Color tint = { 255,255,255,255 }, const Vector2& centerOffset = { 0,0 });
	void Draw(size_t frame, const Vector2& pos, float rotation, const Vector2& size, Color tint = { 255,255,255,255 }, const Vector2& centerOffset = { 0,0 });

	enum class Justifications
	{
		Min,
		Center,
		Max,
	};
	void DrawJustfied(size_t frame, Vector2 pos, Justifications hJustification = Justifications::Center, Justifications vJustitification = Justifications::Center, const Vector2& size = { -1,-1 }, Color tint = { 255,255,255,255 });

	size_t GetRandomAsteroid();

	Color ColorLerp(const Color& start, const Color& end, float param);
}