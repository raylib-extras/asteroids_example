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

void DrawGameHud()
{
	DrawText(TextFormat("Score %d", World::Instance->PlayerShip.Score), 0, 0, 40, BLUE);

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
			float flash = (sinf(GetTime() * 30) * 0.5f) + 0.5f;
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
			float flash = (sinf(GetTime() * 30) * 0.5f) + 0.5f;
			c = Sprites::ColorLerp(WHITE, GRAY, flash);
		}

		Sprites::DrawJustfied(Sprites::ShieldProgress, Vector2{ center, 43 }, Sprites::Justifications::Max, Sprites::Justifications::Min, Vector2{ shieldFactor * 222, 33 }, c);
	}
}

void DrawLevelChangeCountdown()
{
	const char* text = TextFormat("Level Clear, Next Level in %0.0f", std::ceilf(LevelChangeCountdown));
	
	int textSize = 30;
	Vector2 size = MeasureTextEx(GetFontDefault(), text, textSize, textSize / 10);

	Vector2 pos = { GetScreenWidth() / 2.0f - size.x / 2.0f, GetScreenHeight() / 2.0f - size.y / 2.0f };
	DrawText(text, pos.x, pos.y, textSize, WHITE);
}

void DrawGameOver()
{
	const char* text = TextFormat("Game Over, your score was %d, good job!", World::Instance->PlayerShip.Score);

	int textSize = 30;
	Vector2 size = MeasureTextEx(GetFontDefault(), text, textSize, textSize / 10);

	Vector2 pos = { GetScreenWidth() / 2.0f - size.x / 2.0f, GetScreenHeight() / 2.0f - size.y / 2.0f };
	DrawText(text, pos.x, pos.y, textSize, WHITE);

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_SPACE))
	{
		GameState = GameStates::Menu;
		World::Instance->Reset(500);
	}
}

void DrawMenu()
{
	const char* text = "Fasteroids++!, click or press space to play!";

	int textSize = 30;
	Vector2 size = MeasureTextEx(GetFontDefault(), text, textSize, textSize / 10);

	Vector2 pos = { GetScreenWidth() / 2.0f - size.x / 2.0f, GetScreenHeight() / 2.0f - size.y / 2.0f };
	DrawText(text, pos.x, pos.y, textSize, WHITE);

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