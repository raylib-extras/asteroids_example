#include "sprites.h"
#include "raymath.h"


namespace Sprites
{
	std::vector<Texture> SpriteSheets;
	std::vector<SpriteFrame> Frames;
	std::vector<size_t> AsteroidSprites;

	size_t ShipSprite = 0;
	size_t ShotSprite = 0;
	size_t ThrustSprite = 0;
	size_t TurboThustSprite = 0;
	size_t ParticleSprite = 0;

	// UI
	size_t MiniMapSprite = 0;
	size_t ShieldIcon = 0;
	size_t BoostIcon = 0;

	size_t ShieldBar = 0;
	size_t BoostBar = 0;

	size_t ShieldProgress = 0;
	size_t BoostProgress = 0;

	size_t ShieldHitBase = 0;
	size_t ShieldHitMid = 0;
	size_t ShieldHitEnd = 0;

	size_t ShieldPowerup = 0;
	size_t BoostPowerup = 0;
	size_t ShotPowerup = 0;

	size_t Cursor = 0;

	size_t NavArrow = 0;

	size_t AddFrame(size_t sheetId, float x, float y, float w, float h)
	{
		Frames.emplace_back(SpriteFrame(sheetId, x,y,w,h));
		return Frames.size() - 1;
	}

	size_t AddTexture(const char* name)
	{
		Texture tx = LoadTexture(name);
		GenTextureMipmaps(&tx);
		SetTextureFilter(tx, TEXTURE_FILTER_TRILINEAR);

		SpriteSheets.push_back(tx);

		return SpriteSheets.size() - 1;
	}

	void Init()
	{
		AddTexture("sheet.png");
		AddTexture("interfacePack_sheet@2.png");
	
		ShipSprite = AddFrame(0, 224, 832, 99, 75);
		ShotSprite = AddFrame(0, 843, 977, 13, 37);
		ThrustSprite = AddFrame(0, 812, 206, 16, 40);
		TurboThustSprite = AddFrame(0, 827, 867, 16, 38);
		ParticleSprite = AddFrame(0, 364, 814, 18, 18);

		MiniMapSprite = AddFrame(1, 0, 344, 272, 272);
		
		ShieldIcon = AddFrame(0, 482, 325, 34, 33);
		BoostIcon = AddFrame(0, 539, 989, 34, 33);
		BoostBar = AddFrame(0, 0, 78, 222, 39);
		ShieldBar = AddFrame(0, 0, 39, 222, 39);

		ShieldProgress = AddFrame(0, 774, 761, 34, 33);
		Frames[ShieldProgress].Borders = Rectangle{12,12,22,21};

		BoostProgress = AddFrame(0, 696, 329, 34, 33);
		Frames[BoostProgress].Borders = Rectangle{ 12,12,22,21 };

		ShieldHitEnd = AddFrame(0, 0, 412, 133, 108);
		ShieldHitMid = AddFrame(0, 0, 293, 143, 119);
		ShieldHitBase = AddFrame(0, 0, 156, 144, 137);

		ShieldPowerup = AddFrame(0, 222, 129, 22, 21);
		BoostPowerup = AddFrame(0, 674, 262, 22, 21);
		ShotPowerup = AddFrame(0, 222, 108, 22, 21);

		Cursor = AddFrame(0, 382, 814, 17, 17);

		NavArrow = AddFrame(1, 832, 1152, 32, 36);

		AsteroidSprites.push_back(AddFrame(0, 224, 664, 101, 84));
		AsteroidSprites.push_back(AddFrame(0, 0, 520, 120, 98));
		AsteroidSprites.push_back(AddFrame(0, 518, 810, 89, 82));
		AsteroidSprites.push_back(AddFrame(0, 327, 452, 98, 96));
		AsteroidSprites.push_back(AddFrame(0, 651, 447, 43, 43));
		AsteroidSprites.push_back(AddFrame(0, 237, 452, 45, 40));
		AsteroidSprites.push_back(AddFrame(0, 406, 234, 28, 28));
		AsteroidSprites.push_back(AddFrame(0, 778, 587, 29, 26));
		AsteroidSprites.push_back(AddFrame(0, 224, 748, 101, 84));
		AsteroidSprites.push_back(AddFrame(0, 0, 618, 120, 98));
		AsteroidSprites.push_back(AddFrame(0, 516, 728, 89, 82));
		AsteroidSprites.push_back(AddFrame(0, 327, 548, 98, 96));
		AsteroidSprites.push_back(AddFrame(0, 674, 219, 43, 43));
		AsteroidSprites.push_back(AddFrame(0, 282, 452, 45, 40));
		AsteroidSprites.push_back(AddFrame(0, 406, 262, 28, 28));
		AsteroidSprites.push_back(AddFrame(0, 396, 413, 29, 26));
	}

	void Shutdown()
	{
		for (auto& sheet : SpriteSheets)
			UnloadTexture(sheet);

		SpriteSheets.clear();
	}

	void Draw(size_t frameId, const Vector2& pos, float rotation, float size, Color tint, const Vector2& centerOffset)
	{
		Draw(frameId, pos, rotation, Vector2{ size,size }, tint, centerOffset);
	}

	void Draw(size_t frameId, const Vector2& pos, float rotation, const Vector2& size, Color tint, const Vector2& centerOffset)
	{
		const SpriteFrame& frame = Frames[frameId];

		Rectangle dest = { pos.x, pos.y, frame.Frame.width,frame.Frame.height };
		Vector2 center = { frame.Frame.width / 2.0f + centerOffset.x, frame.Frame.height / 2.0f + centerOffset.y };

		if (size.x > 0)
		{
			float aspect = dest.height / dest.width;
			dest.width = size.x;
			dest.height = size.y * aspect;
			center.x = size.x * 0.5f + centerOffset.x;
			center.y = size.y * 0.5f + centerOffset.y;
		}
		DrawTexturePro(SpriteSheets[frame.Sheet], frame.Frame, dest, center, rotation, tint);
	}

	void DrawJustfied(size_t frameId, Vector2 pos, Justifications hJustification, Justifications vJustitification, const Vector2& size, Color tint)
	{
		const SpriteFrame& frame = Frames[frameId];

		Rectangle dest = { pos.x, pos.y, frame.Frame.width,frame.Frame.height };

		if (size.x > 0)
		{
			dest.width = size.x;
			dest.height = size.y;
		}

		switch (hJustification)
		{
			default:
				break;
			case Sprites::Justifications::Center:
				dest.x -= dest.width * 0.5f;
				break;
			case Sprites::Justifications::Max:
				dest.x -= dest.width;
				break;
		}

		switch (vJustitification)
		{
			default:
				break;
			case Sprites::Justifications::Center:
				dest.y -= dest.height * 0.5f;
				break;
			case Sprites::Justifications::Max:
				dest.y -= dest.height;
				break;
		}

		if (frame.Borders.x > 0)
		{
			NPatchInfo info;
			info.source = frame.Frame;
			info.layout = NPATCH_NINE_PATCH;
			info.left = int(frame.Borders.x);
			info.right = int(frame.Frame.width - frame.Borders.width);
			info.top = int(frame.Borders.y);
			info.bottom = int(frame.Frame.height - frame.Borders.height);

			DrawTextureNPatch(SpriteSheets[frame.Sheet], info, dest, Vector2Zero(), 0, tint);
			return;
		}

		DrawTexturePro(SpriteSheets[frame.Sheet], frame.Frame, dest, Vector2Zero(), 0, tint);
	}

	size_t GetRandomAsteroid()
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