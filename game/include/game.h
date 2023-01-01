#pragma once

enum class GameStates
{
	Menu,
	Playing,
	Paused,
	GameOver,
};

extern GameStates GameState;

double GetCurrentTime();
float GetDeltaTime();

void UpdateGame();
void DrawOverlay();