#include "sprites.h"
#include "raymath.h"

namespace Sprites
{
	Texture SpriteSheet = { 0 };
	Texture RadarRing = { 0 };
	std::vector<Rectangle> AsteroidSprites;
	Rectangle ShipSprite = { 6 * 128, 0 * 128 , 128, 128 };
	Rectangle ShotSprite = { 7 * 128, 3 * 128, 128, 128 };
	Rectangle ThrustSprite = { 7 * 128, 5 * 128, 128, 128 };
	Rectangle TurboThustSprite = { 6 * 128, 5 * 128, 128, 128 };
	Rectangle ParticleSprite = { 4 * 128, 3 * 128, 128, 128 };

	void Init()
	{
		RadarRing = LoadTexture("resources/mini-map-ring.png");
		SpriteSheet = LoadTexture("resources/simpleSpace_tilesheet@2.png");
		GenTextureMipmaps(&SpriteSheet);
		SetTextureFilter(SpriteSheet, TEXTURE_FILTER_TRILINEAR);

		AsteroidSprites.push_back(Rectangle{ 0 * 128, 3 * 128, 128, 128 });
		AsteroidSprites.push_back(Rectangle{ 2 * 128, 3 * 128, 128, 128 });
		AsteroidSprites.push_back(Rectangle{ 0 * 128, 4 * 128, 128, 128 });
		AsteroidSprites.push_back(Rectangle{ 2 * 128, 4 * 128, 128, 128 });
	}

	void Shutdown()
	{
		UnloadTexture(RadarRing);
		UnloadTexture(SpriteSheet);
		SpriteSheet.id = 0;
	}

	void Draw(Rectangle frame, Vector2 pos, float rotation, float size, Color tint)
	{
		Rectangle dest = { pos.x, pos.y, frame.width,frame.height };
		Vector2 center = { frame.width / 2.0f, frame.height / 2.0f };

		if (size > 0)
		{
			dest.width = dest.height = size;
			center.x = center.y = size * 0.5f;
		}

		DrawTexturePro(SpriteSheet, frame, dest, center, rotation, tint);
	}

	Rectangle GetRandomAsteroid()
	{
		return AsteroidSprites[GetRandomValue(0, (int)AsteroidSprites.size() - 1)];
	}

	Color ColorLerp(const Color& start, const Color& end, float param)
	{
		Color c;
		c.r = (unsigned char)Lerp(start.r, end.r, param);
		c.g = (unsigned char)Lerp(start.g, end.g, param);
		c.b = (unsigned char)Lerp(start.b, end.b, param);
		c.a = (unsigned char)Lerp(start.a, end.a, param);

		return c;
	}
}