#pragma once

#include "raylib.h"

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

Vector2 GetDisplaySize();
void ToggleFullscreenState();
void CenterWindow();

constexpr int WindowWidth = 1280;
constexpr int WindowHeight = 800;