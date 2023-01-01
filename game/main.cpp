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
#include "world.h"

#include <vector>

Texture Background = { 0 };

void DrawStarfield(const Camera2D& camera)
{
	Rectangle screen = { 0,0,  (float)GetScreenWidth(),  (float)GetScreenHeight() };

	float bgScale = 0.5f;

	Vector2 screenOriginInWorld = GetScreenToWorld2D(Vector2Zero(), camera);
	Vector2 screenEdgeInWorld = GetScreenToWorld2D(Vector2{ screen.width,screen.height }, camera);

	Rectangle screenInWorld = Rectangle{ screenOriginInWorld.x, screenOriginInWorld.y, screenEdgeInWorld.x - screenOriginInWorld.x,screenEdgeInWorld.y - screenOriginInWorld.y };

	Rectangle sourceRect = Rectangle{ screenInWorld.x * bgScale, screenInWorld.y * bgScale, screenInWorld.width * bgScale, screenInWorld.width * bgScale };
	DrawTexturePro(Background, sourceRect, screenInWorld, Vector2Zero(), 0, WHITE);
}

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

	camera.offset.x += sinf(float(GetCurrentTime() * 90)) * 2;
	camera.offset.y += sinf(float(GetCurrentTime() * 180)) * 2;
}

int main ()
{
	// set up the window
	SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow(1280, 800, "Fasteroids++");
	SetFPSCap();

	Sprites::Init();

	Camera2D worldCamera = { 0 };
	worldCamera.zoom = 0.25f;
	worldCamera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

	Camera2D overlayCamera = { 0 };
	overlayCamera.zoom = 1.0f;

	Background = LoadTexture("resources/blue.png");

	World world;
	int level = 1;

	GameState = GameStates::Playing;
	world.Reset(level);

	// game loop
	while (!WindowShouldClose())
	{
		UpdateGame();

		if (IsKeyDown(KEY_EQUAL))
		{
			worldCamera.zoom += 0.125f * GetDeltaTime();

			if (worldCamera.zoom > 1)
				worldCamera.zoom = 1;
		}
		if (IsKeyDown(KEY_MINUS))
		{
			worldCamera.zoom -= 0.125f * GetDeltaTime();
			if (worldCamera.zoom <= 0)
				worldCamera.zoom = 0.125f;
		}

		world.Update();

		worldCamera.target = world.PlayerShip.Position;

		ApplyViewCameraEffects(worldCamera, world.PlayerShip.Turbo);

		// drawing
		BeginDrawing();
		ClearBackground(BLACK);

		BeginMode2D(worldCamera);

		DrawStarfield(worldCamera);

		world.Draw();

		EndMode2D();
		
		ApplyOverlayCameraEffects(overlayCamera, world.PlayerShip.Turbo);
		BeginMode2D(overlayCamera);
		DrawOverlay();
		EndMode2D();
		
		EndDrawing();
	}

	UnloadTexture(Background);
	Sprites::Shutdown();

	// cleanup
	CloseWindow();
	return 0;
}