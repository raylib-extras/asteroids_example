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