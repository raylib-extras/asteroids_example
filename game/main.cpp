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

#include <vector>

Texture Background = { 0 };

void ApplyViewCameraEffects(Camera2D& camera, bool turbo)
{
	camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	if (!turbo)
		return;

	camera.offset.x += sinf(float(GetCurrentTime() * 90)) * 2;
	camera.offset.y += sinf(float(GetCurrentTime() * 180)) * 2;
}

void ApplyOverlayCameraEffects(Camera2D& camera, bool turbo)
{
	camera.offset = { 0,0 };
	if (!turbo)
		return;

	camera.offset.x += sinf(float(GetCurrentTime() * 90)) * 3;
	camera.offset.y += sinf(float(GetCurrentTime() * 180)) * 3;
}

int main ()
{
	// set up the window
	SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow(1280, 800, "Fasteroids++");
	SetFPSCap();

	SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);

	Sounds::Init();
	SetMasterVolume(0.5f);

	Sprites::Init();

	Camera2D worldCamera = { 0 };
	worldCamera.zoom = 0.5f;
	worldCamera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

	Camera2D overlayCamera = { 0 };
	overlayCamera.zoom = 1.0f;

	Background = LoadTexture("resources/darkPurple.png");

	World world;

	GameState = GameStates::Menu;
	world.PlayerShip.Reset();
	world.PlayerShip.Alive = false;
	world.Reset(50);

	Sounds::StartBGM();

	// game loop
	while (!WindowShouldClose())
	{
		UpdateGame();
		Sounds::Update();

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

		ApplyViewCameraEffects(worldCamera, world.PlayerShip.Boost);

		// drawing
		BeginDrawing();
		ClearBackground(BLACK);

		BeginMode2D(worldCamera);

		Rectangle screen = { 0,0,  (float)GetScreenWidth(),  (float)GetScreenHeight() };
		Vector2 screenOriginInWorld = GetScreenToWorld2D(Vector2Zero(), worldCamera);
		Vector2 screenEdgeInWorld = GetScreenToWorld2D(Vector2{ screen.width,screen.height }, worldCamera);
		Rectangle screenInWorld = Rectangle{ screenOriginInWorld.x, screenOriginInWorld.y, screenEdgeInWorld.x - screenOriginInWorld.x,screenEdgeInWorld.y - screenOriginInWorld.y };

		float bgScale = 0.5f;
		Rectangle sourceRect = Rectangle{ screenInWorld.x * bgScale, screenInWorld.y * bgScale, screenInWorld.width * bgScale, screenInWorld.width * bgScale };
		DrawTexturePro(Background, sourceRect, screenInWorld, Vector2Zero(), 0, WHITE);

		world.Draw(screenInWorld);

		EndMode2D();
		
		ApplyOverlayCameraEffects(overlayCamera, world.PlayerShip.Boost);
		BeginMode2D(overlayCamera);
		DrawOverlay();
		EndMode2D();
		
		EndDrawing();
	}

	UnloadTexture(Background);
	Sprites::Shutdown();

	Sounds::Shutdown();

	// cleanup
	CloseWindow();
	return 0;
}