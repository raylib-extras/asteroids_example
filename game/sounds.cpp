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

#include "sounds.h"

#include <vector>

namespace Sounds
{
	std::vector<Sound> Effects;

	Music BGM;
	Music Thrust;
	Music Boost;

	size_t AsteroidHit = 0;
	size_t Destoryed = 0;
	size_t SheldHit = 0;
	size_t Shot = 0;
	size_t Upgrade = 0;

	size_t GameOver = 0;
	size_t Begin = 0;

	size_t AddEffect(const char* name)
	{
		Effects.emplace_back(LoadSound(name));
		return Effects.size() - 1;
	}

	void Init()
	{
		InitAudioDevice();
		BGM = LoadMusicStream("bgm.ogg");
		BGM.looping = true;
		SetMusicVolume(BGM, 0.25f);

		Thrust = LoadMusicStream("thruster.ogg");
		Thrust.looping = true;
		SetMusicVolume(Thrust, 1.0f);

		Boost = LoadMusicStream("boost.ogg");
		Boost.looping = true;

		AsteroidHit = AddEffect("asteroid_hit.ogg");
		Destoryed = AddEffect("destoryed.ogg");
		SheldHit = AddEffect("shield_hit.ogg");
		Shot = AddEffect("shot.ogg");
		Upgrade = AddEffect("upgrade.ogg");
		GameOver = AddEffect("game_over.ogg");
		Begin = AddEffect("begin.ogg");
	}

	void Shutdown()
	{
		if (IsMusicStreamPlaying(BGM))
			StopMusicStream(BGM);

		if (IsMusicStreamPlaying(Thrust))
			StopMusicStream(Thrust);

		if (IsMusicStreamPlaying(BGM))
			StopMusicStream(Thrust);

		CloseAudioDevice();

		for (const auto& effect : Effects)
			UnloadSound(effect);

		Effects.clear();
	}

	void Update()
	{
		if (IsMusicStreamPlaying(BGM))
		UpdateMusicStream(BGM);

		if (IsMusicStreamPlaying(Thrust))
			UpdateMusicStream(Thrust);

		if (IsMusicStreamPlaying(Boost))
			UpdateMusicStream(Boost);
	}

	void SetThrustState(bool thrusting, bool boosting)
	{
		if (!thrusting)
		{
			if (IsMusicStreamPlaying(Thrust))
				StopMusicStream(Thrust);

			if (IsMusicStreamPlaying(Boost))
				StopMusicStream(Boost);
		}
		else
		{
			if (!IsMusicStreamPlaying(Thrust))
				PlayMusicStream(Thrust);

			if (boosting)
			{
				if (!IsMusicStreamPlaying(Boost))
					PlayMusicStream(Boost);
			}
			else
			{
				if (IsMusicStreamPlaying(Boost))
					StopMusicStream(Boost);
			}
		}
	}

	void StartBGM()
	{
		if (!IsMusicStreamPlaying(BGM))
			PlayMusicStream(BGM);
	}

	void StopBGM() 
	{
		if (IsMusicStreamPlaying(BGM))
			StopMusicStream(BGM);
	}

	void PlaySoundEffect(size_t effect)
	{
		if (effect <= Effects.size())
			PlaySoundMulti(Effects[effect]);
	}
}