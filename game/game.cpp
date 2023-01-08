#include "game.h"
#include "common.h"
#include "world.h"
#include "sprites.h"
#include "sounds.h"

#include <cmath>

constexpr char Version[] = "Ver 0.0.3.Petty_Dragon Copyright 2023 Jeffery Myers";

constexpr float LevelChangeTime = 10;

GameStates GameState = GameStates::Menu;

double Time = 0;

int Level = 1;
float LevelChangeCountdown = LevelChangeTime;

Camera2D OverlayCamera = { 0 };

bool Running = true;

Vector2 GetDisplaySize()
{
	if (IsWindowFullscreen())
		return Vector2{ (float)GetMonitorWidth(GetCurrentMonitor()), (float)GetMonitorHeight(GetCurrentMonitor()) };
	else
		return Vector2{ (float)GetScreenWidth(), (float)GetScreenHeight() };
}

void ToggleFullscreenState()
{
	if (IsWindowFullscreen())
	{
		ToggleFullscreen();
		SetWindowSize(WindowWidth, WindowHeight);
		CenterWindow();
	}
	else
	{
		int monitor = GetCurrentMonitor();
		SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
		ToggleFullscreen();
	}
}

void CenterWindow()
{
	int monitor = GetCurrentMonitor();

	int x = GetMonitorWidth(monitor) / 2 - WindowWidth / 2;
	int y = GetMonitorHeight(monitor) / 2 - WindowHeight / 2;
	
	SetWindowPosition(x, y);
}

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

void EndGame()
{
	GameState = GameStates::GameOver;
	Sounds::SetThrustState(false, false);
	World::Instance->PlayerShip.Alive = false;
}

bool UpdateGame()
{
	if (GameState != GameStates::Paused)
		Time += GetDeltaTime();

	if (GameState == GameStates::Paused)
	{
		if (World::Instance->PlayerShip.AcceptPressed())
			GameState = GameStates::Playing;
		else if (IsKeyPressed(KEY_ESCAPE))
			EndGame();
	}

	if (GameState == GameStates::ChangingLevels)
	{
		LevelChangeCountdown -= GetDeltaTime();
		if (LevelChangeCountdown < 0)
		{
			GameState = GameStates::Playing;
			World::Instance->Reset(Level);
		}

		if (IsKeyPressed(KEY_ESCAPE))
			GameState = GameStates::Paused;
	}

	if (GameState == GameStates::Playing)
	{
		if (!World::Instance->PlayerShip.Alive)
		{
			Sounds::PlaySoundEffect(Sounds::GameOver);
			EndGame();
			
		}
		else if(World::Instance->IsLevelClear())
		{
			GameState = GameStates::ChangingLevels;
			LevelChangeCountdown = LevelChangeTime;
			Level += 1;
		}

		if (IsKeyPressed(KEY_ESCAPE))
			GameState = GameStates::Paused;
	}

	return Running;
}

void DrawCenteredText(const char* text, float textSize = 20, float yOffset = 0.5f, float xOffset = 0.5f)
{
	Vector2 size = MeasureTextEx(GetFontDefault(), text, textSize, textSize / 10);

	Vector2 pos = { GetDisplaySize().x * xOffset - size.x / 2.0f, GetDisplaySize().y * yOffset - size.y / 2.0f };
	DrawText(text, int(pos.x), int(pos.y), int(textSize), WHITE);
}

void DrawMiniMap()
{
	Vector2 center = { Sprites::Frames[Sprites::MiniMapSprite].Frame.width, Sprites::Frames[Sprites::MiniMapSprite].Frame.height };

	float rad = center.x * 0.5f - 10;

	center.x = GetDisplaySize().x - (center.x / 2.0f);
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

	for (const auto& powerup : World::Instance->PowerUps)
	{
		if (!powerup.Alive || Vector2DistanceSqr(World::Instance->PlayerShip.Position, powerup.Position) >= viewDist * viewDist)
			continue;

		Vector2 relPos = Vector2Subtract(powerup.Position, World::Instance->PlayerShip.Position);
		relPos = Vector2Scale(relPos, viewScale);
		relPos = Vector2Add(relPos, center);

		DrawCircleV(relPos, 1, PURPLE);
	}

	Vector2 upperRight = { float(GetDisplaySize().x),0 };
	Sprites::DrawJustfied(Sprites::MiniMapSprite, upperRight, Sprites::Justifications::Max, Sprites::Justifications::Min);

	if (World::Instance->GetActiveAsteroidCount() < 6)
	{
		for (const auto& asteroid : World::Instance->Asteroids)
		{
			if (!asteroid.Alive)
				continue;

			Vector2 relPos = Vector2Subtract(asteroid.Position, World::Instance->PlayerShip.Position);
			float angle = atan2f(relPos.y, relPos.x) * RAD2DEG + 90;
			Sprites::Draw(Sprites::NavArrow, center, angle, Vector2{ 16,16 }, RED, Vector2{ 0,rad - 8 });
		}
	}
}

void DrawGameHud()
{
	OverlayCamera.zoom = 1.0f;
	if (World::Instance->Shake())
		OverlayCamera.offset = Vector2{ sinf(float(GetCurrentTime() * 90)) * 3, sinf(float(GetCurrentTime() * 180)) * 3 };
	else
		OverlayCamera.offset = Vector2Zero();

	BeginMode2D(OverlayCamera);
	DrawText(TextFormat("Level %d | Score %d", Level, World::Instance->PlayerShip.GetScore()), 0, 0, 40, BLUE);

	DrawMiniMap();
	if (World::Instance->GetActiveAsteroidCount() < 10)
	{
		DrawCenteredText(TextFormat("Asteroids Left : %d", World::Instance->GetActiveAsteroidCount()), 20, 0.125f);
	}

	float topBarWidth = 222+33;
	float center = GetDisplaySize().x / 2.0f - topBarWidth / 2.0f;

	Sprites::DrawJustfied(Sprites::BoostIcon, Vector2{ center, 3 }, Sprites::Justifications::Min, Sprites::Justifications::Min);
	Sprites::DrawJustfied(Sprites::ShieldIcon, Vector2{ center, 43 }, Sprites::Justifications::Min, Sprites::Justifications::Min);

	center = GetDisplaySize().x / 2.0f + topBarWidth / 2.0f;

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
	EndMode2D();
}

void DrawLevelChangeCountdown()
{
	DrawCenteredText(TextFormat("Level Clear, Next Level in %0.0f", (float)std::ceil(LevelChangeCountdown)), 20, 0.25f);
}

void DrawGameOver()
{
	DrawCenteredText(TextFormat("Game Over, your score was %d, good job!", World::Instance->PlayerShip.GetScore()), 20, 0.25f);
	
	if (World::Instance->PlayerShip.AcceptPressed())
	{
		GameState = GameStates::Menu;
		World::Instance->Reset(10);
	}
}

void DrawPaused()
{
	DrawCenteredText("Paused, escape to quit, click to resume", 40, 0.25f);
}

void DrawMenu()
{
	DrawCenteredText("Fasteroids++!", 60, 0.125f);

	DrawCenteredText("Click or press space to play!", 20, 0.25f);

	DrawCenteredText("Instructions", 20, 0.5f);
	DrawCenteredText("Mouse to steer, W or right click to thrust", 20, 0.55f);
	DrawCenteredText("Left Click or space to fire", 20, 0.6f);
	DrawCenteredText("Shift + Thrust to Boost", 20, 0.65f);

	if (IsKeyPressed(KEY_ESCAPE))
		Running = false;
	
	if (World::Instance->PlayerShip.AcceptPressed())
	{
		Level = 1;
		GameState = GameStates::Playing;
		World::Instance->PlayerShip.Reset();
		World::Instance->Reset(Level);

		Sounds::PlaySoundEffect(Sounds::Begin);
	}

	DrawText(Version, 0, 0, 20, GRAY);
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
			DrawPaused();
			break;

		case GameStates::GameOver:
			DrawGameHud();
			DrawGameOver();
			break;
	}

	DrawFPS(0, (int)GetDisplaySize().y - 20);
}