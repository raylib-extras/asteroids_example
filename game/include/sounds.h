#pragma once

#include "raylib.h"

namespace Sounds
{
	extern size_t AsteroidHit;
	extern size_t Destoryed;
	extern size_t SheldHit;
	extern size_t Shot;

	void Init();
	void Shutdown();
	void Update();

	void SetThrustState(bool thrusting, bool boosting);

	void StartBGM();
	void StopBGM();

	void PlaySoundEffect(size_t effect);
}