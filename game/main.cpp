/*
Quick Asterpids in C++

*/

#include "raylib.h"
#include "raymath.h"

#include "asteroid.h"
#include "bullet.h"
#include "common.h"
#include "entity.h"
#include "explosion.h"
#include "game.h"
#include "player.h"
#include "sprites.h"
#include "sounds.h"
#include "world.h"
#include "resource_dir.h"

#include <vector>

Texture Background = { 0 };

int main ()
{
	// set up the window
	SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow(WindowWidth, WindowHeight, "Fasteroids++");
	CenterWindow();
	SetFPSCap();

	SearchAndSetResourceDir("resources");

	HideCursor();
	//SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);

	Sounds::Init();
	SetMasterVolume(0.5f);

	Sprites::Init();

	Camera2D worldCamera = { 0 };
	worldCamera.zoom = 0.5f;
	
	Background = LoadTexture("darkPurple.png");

	World world;

	GameState = GameStates::Menu;
	world.PlayerShip.Reset();
	world.PlayerShip.Alive = false;
	world.Reset(10);

	Sounds::StartBGM();

	// game loop
	while (!WindowShouldClose())
	{
		UpdateGame();
		Sounds::Update();

		if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))
			ToggleFullscreenState();
			
		Vector2 center = Vector2Scale(GetDisplaySize(), 0.5f);
		worldCamera.offset = center;

		if (IsKeyDown(KEY_EQUAL))
			worldCamera.zoom += 0.125f * GetDeltaTime();
		if (IsKeyDown(KEY_MINUS))
			worldCamera.zoom -= 0.125f * GetDeltaTime();

		worldCamera.zoom += GetMouseWheelMove() * 0.125f * GetDeltaTime();

		if (worldCamera.zoom <= 0)
			worldCamera.zoom = 0.25f;

		if (worldCamera.zoom > 1)
			worldCamera.zoom = 1;

		world.Update();

		worldCamera.target = world.PlayerShip.Position;

		// drawing
		BeginDrawing();
		ClearBackground(BLACK);

		if (World::Instance->Shake())
			worldCamera.offset = Vector2{ center.x + cosf(float(GetCurrentTime() * 90)) * 2, center.y + cosf(float(GetCurrentTime() * 180)) * 2 };
		else
			worldCamera.offset = center;

		BeginMode2D(worldCamera);

		Rectangle screen = { 0,0,  center.x*2,  center.y*2 };
		Vector2 screenOriginInWorld = GetScreenToWorld2D(Vector2Zero(), worldCamera);
		Vector2 screenEdgeInWorld = GetScreenToWorld2D(Vector2{ screen.width, screen.height }, worldCamera);
		Rectangle screenInWorld = Rectangle{ screenOriginInWorld.x, screenOriginInWorld.y, screenEdgeInWorld.x - screenOriginInWorld.x,screenEdgeInWorld.y - screenOriginInWorld.y };

		float bgScale = 0.5f;
		Rectangle sourceRect = Rectangle{ screenInWorld.x * bgScale, screenInWorld.y * bgScale, screenInWorld.width * bgScale, screenInWorld.height * bgScale };
		DrawTexturePro(Background, sourceRect, screenInWorld, Vector2Zero(), 0, WHITE);

		world.Draw(screenInWorld);

		EndMode2D();
		
		DrawOverlay();

		Sprites::Draw(Sprites::Cursor, GetMousePosition(), 0);

		EndDrawing();
	}

	UnloadTexture(Background);
	Sprites::Shutdown();

	Sounds::Shutdown();

	// cleanup
	CloseWindow();
	return 0;
}