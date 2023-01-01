#pragma once

#include "raylib.h"

#include <vector>

namespace Sprites
{
	extern Texture SpriteSheet;
	extern std::vector<Rectangle> AsteroidSprites;
	extern Rectangle ShipSprite;
	extern Rectangle ShotSprite;
	extern Rectangle ThrustSprite;
	extern Rectangle TurboThustSprite;
	extern Rectangle ParticleSprite;

	extern Texture RadarRing;

	void Init();
	void Shutdown();

	void Draw(Rectangle frame, Vector2 pos, float rotation = 0, float size = -1, Color tint = { 255,255,255,255 });

	Rectangle GetRandomAsteroid();

	Color ColorLerp(const Color& start, const Color& end, float param);
}