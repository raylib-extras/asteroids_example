#pragma once

enum class GameStates
{
	Menu,
	Playing,
	ChangingLevels,
	Paused,
	GameOver,
};

extern GameStates GameState;

double GetCurrentTime();
float GetDeltaTime();

void UpdateGame();
void DrawOverlay();