#include "game.h"
#include "common.h"
#include "world.h"
#include "sprites.h"

#include <cmath>

constexpr float LevelChangeTime = 10;

GameStates GameState = GameStates::Menu;

double Time = 0;

int Level = 1;
float LevelChangeCountdown = LevelChangeTime;

double GetCurrentTime()
{
	return Time;
}

float GetDeltaTime()
{
	if (GameState == GameStates::Paused)
		return 0;

#ifdef _DEBUG
	return 1.0f / FIXED_FRAME_RATE;
#else
	return GetFrameTime();
#endif
}

void UpdateGame()
{
	if (GameState != GameStates::Paused)
		Time += GetDeltaTime();

	if (GameState == GameStates::ChangingLevels)
	{
		LevelChangeCountdown -= GetDeltaTime();
		if (LevelChangeCountdown < 0)
		{
			GameState = GameStates::Playing;
			World::Instance->Reset(Level);
		}
	}

	if (GameState == GameStates::Playing)
	{
		if (!World::Instance->PlayerShip.Alive)
		{
			GameState = GameStates::GameOver;
		}
		else if(World::Instance->IsLevelClear())
		{
			GameState = GameStates::ChangingLevels;
			LevelChangeCountdown = LevelChangeTime;
			Level += 1;
		}
	}
}

void DrawCenteredText(const char* text, float textSize = 20, float yOffset = 0.5f, float xOffset = 0.5f)
{
	Vector2 size = MeasureTextEx(GetFontDefault(), text, textSize, textSize / 10);

	Vector2 pos = { GetScreenWidth() * xOffset - size.x / 2.0f, GetScreenHeight() * yOffset - size.y / 2.0f };
	DrawText(text, int(pos.x), int(pos.y), int(textSize), WHITE);
}

void DrawMiniMap()
{
	Vector2 center = { Sprites::Frames[Sprites::MiniMapSprite].Frame.width, Sprites::Frames[Sprites::MiniMapSprite].Frame.height };

	float rad = center.x * 0.5f - 10;

	center.x = GetScreenWidth() - (center.x / 2.0f);
	center.y = center.y / 2.0f;

	DrawCircleV(center, rad, ColorAlpha(BLACK, 0.75f));

	DrawCircleV(center, 5, WHITE);

	float viewDist = 3000;

	float viewScale = rad / viewDist;
	for (const auto& asteroid : World::Instance->Asteroids)
	{
		if (!asteroid.Alive || Vector2DistanceSqr(World::Instance->PlayerShip.Position, asteroid.Position) >= viewDist * viewDist)
			continue;

		Vector2 relPos = Vector2Subtract(asteroid.Position, World::Instance->PlayerShip.Position);
		relPos = Vector2Scale(relPos, viewScale);
		relPos = Vector2Add(relPos, center);

		DrawCircleV(relPos, 2, BROWN);
	}
}

void DrawGameHud()
{
	DrawText(TextFormat("Score %d", World::Instance->PlayerShip.Score), 0, 0, 40, BLUE);

	DrawMiniMap();
	Vector2 upperRight = { float(GetScreenWidth()),0 };
	Sprites::DrawJustfied(Sprites::MiniMapSprite, upperRight, Sprites::Justifications::Max, Sprites::Justifications::Min);

	float topBarWidth = 222+33;
	float center = GetScreenWidth()/ 2.0f - topBarWidth / 2.0f;

	Sprites::DrawJustfied(Sprites::BoostIcon, Vector2{ center, 3 }, Sprites::Justifications::Min, Sprites::Justifications::Min);
	Sprites::DrawJustfied(Sprites::ShieldIcon, Vector2{ center, 43 }, Sprites::Justifications::Min, Sprites::Justifications::Min);

	center = GetScreenWidth() / 2.0f + topBarWidth / 2.0f;

	float boostFactor = World::Instance->PlayerShip.Power / 1000.0f;
	float shieldFactor = World::Instance->PlayerShip.Shield / 1000.0f;

	Sprites::DrawJustfied(Sprites::BoostBar, Vector2{ center, 0 }, Sprites::Justifications::Max, Sprites::Justifications::Min, Vector2{-1,-1}, ColorAlpha(WHITE, 0.5f));
	if (boostFactor > 0)
	{
		Color c = WHITE;
		if (boostFactor < 0.25f)
		{
			float flash = (sinf((float)GetTime() * 30) * 0.5f) + 0.5f;
			c = Sprites::ColorLerp(WHITE, GRAY, flash);
		}
		Sprites::DrawJustfied(Sprites::BoostProgress, Vector2{ center, 3 }, Sprites::Justifications::Max, Sprites::Justifications::Min, Vector2{ boostFactor * 222, 33 }, c);
	}

	Sprites::DrawJustfied(Sprites::ShieldBar, Vector2{ center, 40 }, Sprites::Justifications::Max, Sprites::Justifications::Min, Vector2{ -1,-1 }, ColorAlpha(WHITE, 0.5f));

	if (shieldFactor > 0)
	{
		Color c = WHITE;
		if (shieldFactor < 0.25f)
		{
			float flash = (sinf((float)GetTime() * 30) * 0.5f) + 0.5f;
			c = Sprites::ColorLerp(WHITE, GRAY, flash);
		}

		Sprites::DrawJustfied(Sprites::ShieldProgress, Vector2{ center, 43 }, Sprites::Justifications::Max, Sprites::Justifications::Min, Vector2{ shieldFactor * 222, 33 }, c);
	}
}

void DrawLevelChangeCountdown()
{
	DrawCenteredText(TextFormat("Level Clear, Next Level in %0.0f", std::ceilf(LevelChangeCountdown)), 20, 0.25f);
}

void DrawGameOver()
{
	DrawCenteredText(TextFormat("Game Over, your score was %d, good job!", World::Instance->PlayerShip.Score), 20, 0.25f);
	
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_SPACE))
	{
		GameState = GameStates::Menu;
		World::Instance->Reset(50);
	}
}

void DrawMenu()
{
	DrawCenteredText("Fasteroids++!", 60, 0.125f);

	DrawCenteredText("Click or press space to play!", 20, 0.25f);

	DrawCenteredText("Instructions", 20, 0.5f);
	DrawCenteredText("Mouse to steer, W or right click to thrust", 20, 0.55f);
	DrawCenteredText("Left Click or space to fire", 20, 0.6f);
	DrawCenteredText("Shift + Thrust to Boost", 20, 0.65f);
	
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_SPACE))
	{
		Level = 1;
		GameState = GameStates::Playing;
		World::Instance->PlayerShip.Reset();
		World::Instance->Reset(Level);
	}
}

void DrawOverlay()
{
	switch (GameState)
	{
		case GameStates::Menu:
			DrawMenu();
			break;

		case GameStates::Playing: 
			DrawGameHud();
			break;

		case GameStates::ChangingLevels:
			DrawGameHud();
			DrawLevelChangeCountdown();
			break;

		case GameStates::Paused:
			DrawGameHud();
			break;

		case GameStates::GameOver:
			DrawGameHud();
			DrawGameOver();
			break;
	}

	DrawFPS(0, GetScreenHeight() - 20);
}