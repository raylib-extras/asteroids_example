#pragma once

#include "raylib.h"
#include "raymath.h"

#include "game.h"

#include <stdlib.h>

constexpr int FIXED_FRAME_RATE = 60;

inline void SetFPSCap()
{
#ifdef _DEBUG
	SetTargetFPS(FIXED_FRAME_RATE);
#else
	SetTargetFPS(144);
#endif
}

inline float GetRandomValueF(float min, float max)
{
	if (min > max)
	{
		float tmp = max;
		max = min;
		min = tmp;
	}
	double param = rand() / (double)RAND_MAX;
	return float(min + (max - min) * param);
}

inline Vector2 GetRandomVector2(const Rectangle& range)
{
	return Vector2{ GetRandomValueF(range.x,range.x + range.width),GetRandomValueF(range.y,range.y + range.height) };
}