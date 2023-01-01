#include "game.h"
#include "common.h"
#include "world.h"
#include "sprites.h"

GameStates GameState = GameStates::Menu;

double Time = 0;

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
}

void DrawOverlay()
{
	switch (GameState)
	{
		case GameStates::Menu:
			break;

		case GameStates::Playing: 
		{
			DrawText(TextFormat("Score %d", World::Instance->PlayerShip.Score), 600, 0, 20, BLUE);

			float scale = 0.5f;
			float size = float(Sprites::RadarRing.width * scale);

			DrawTextureEx(Sprites::RadarRing, Vector2{ float(GetScreenWidth() - size), 0 }, 0, scale, ColorAlpha(WHITE,0.5f));
		}
			break;

		case GameStates::Paused:
			break;

		case GameStates::GameOver:
			break;
	}

	DrawFPS(0, 0);
}