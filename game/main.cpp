/**********************************************************************************************
*
*   Raylib Asteroids Example 'fasteroids++' * A simple RPG made using raylib
*
*    LICENSE: zlib/libpng
*
*   Copyright (c) 2023 Jeffery Myers
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"
#include "raymath.h"

#include "asteroid.h"
#include "bullet.h"
#include "common.h"
#include "entity.h"
#include "explosion.h"
#include "game.h"
#include "player.h"
#include "resource_dir.h"
#include "sounds.h"
#include "sprites.h"
#include "world.h"

#include <vector>


Texture Background = { 0 };	// texture to use for the background image

int main ()
{
	// set up the window
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(WindowWidth, WindowHeight, "Fasteroids++");
	CenterWindow();
	SetFPSCap();
	SetExitKey(KEY_NULL);


	// find our resource dir and set it as the current working dir
	SearchAndSetResourceDir("resources");

	// hide the OS cursor, we are going to draw our own
	HideCursor();

	// set up sounds
	Sounds::Init();
	SetMasterVolume(0.5f);

	// load images
	Sprites::Init();
	Background = LoadTexture("darkPurple.png");

	// setup the camera for the game world
	Camera2D worldCamera = { 0 };
	worldCamera.zoom = 0.5f;
	
	// setup world
	World& world = World::Create();
	world.PlayerShip.Reset();
	world.PlayerShip.Alive = false;
	world.Reset(10);

	// setup gamestate
	GameState = GameStates::Menu;

	// start some music
	Sounds::StartBGM();

	// game loop
	while (!WindowShouldClose() && UpdateGame())
	{
		// input that is unrelated to the game

		// check for fullscreen toggle
		if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))
			ToggleFullscreenState();
			
		// update the world camera
		Vector2 center = Vector2Scale(GetRenderSize(), 0.5f);
		worldCamera.offset = center;

		if (IsKeyDown(KEY_EQUAL))
			worldCamera.zoom += 0.125f * GetDeltaTime();
		if (IsKeyDown(KEY_MINUS))
			worldCamera.zoom -= 0.125f * GetDeltaTime();

		worldCamera.zoom += GetMouseWheelMove() * 0.125f * GetDeltaTime();

		// clamp the zoom
		if (worldCamera.zoom <= 0)
			worldCamera.zoom = 0.25f;

		if (worldCamera.zoom > 1)
			worldCamera.zoom = 1;

		// update the world and all that is in it
		world.Update();
		worldCamera.target = world.PlayerShip.Position;

		// update the sound system
		Sounds::Update();

		// drawing
		BeginDrawing();
		ClearBackground(BLACK);

		// if we need to shake, shake the world camera
		if (World::Instance->Shake())
			worldCamera.offset = Vector2{ center.x + cosf(float(GetCurrentTime() * 90)) * 2, center.y + cosf(float(GetCurrentTime() * 180)) * 2 };
		else
			worldCamera.offset = center;

		// draw the world inside it's view
		BeginMode2D(worldCamera);

		// compute the size of the background and shift it based on our movement
		Rectangle screen = { 0,0,  center.x*2,  center.y*2 };
		Vector2 screenOriginInWorld = GetScreenToWorld2D(Vector2Zero(), worldCamera);
		Vector2 screenEdgeInWorld = GetScreenToWorld2D(Vector2{ screen.width, screen.height }, worldCamera);
		Rectangle screenInWorld = Rectangle{ screenOriginInWorld.x, screenOriginInWorld.y, screenEdgeInWorld.x - screenOriginInWorld.x,screenEdgeInWorld.y - screenOriginInWorld.y };

		float bgScale = 0.5f;
		Rectangle sourceRect = Rectangle{ screenInWorld.x * bgScale, screenInWorld.y * bgScale, screenInWorld.width * bgScale, screenInWorld.height * bgScale };
		DrawTexturePro(Background, sourceRect, screenInWorld, Vector2Zero(), 0, WHITE);

		// draw the world and pass in the viewport window in world space for culling
		world.Draw(screenInWorld);

		EndMode2D();
		
		// draw the game state specific overlay
		DrawOverlay();

		// custom cursor may not work in fullscreen, so draw our own after everything
		Sprites::Draw(Sprites::Cursor, GetMousePosition(), 0);

		EndDrawing();
	}

	// cleanup
	UnloadTexture(Background);
	Sprites::Shutdown();
	Sounds::Shutdown();
	World::Destory();

	CloseWindow();
	return 0;
}