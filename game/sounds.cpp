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

	size_t AddEffect(const char* name)
	{
		Effects.emplace_back(LoadSound(name));
		return Effects.size() - 1;
	}

	void Init()
	{
		InitAudioDevice();
		BGM = LoadMusicStream("resources/bgm.ogg");
		BGM.looping = true;

		Thrust = LoadMusicStream("resources/thruster.ogg");
		Thrust.looping = true;

		Boost = LoadMusicStream("resources/boost.ogg");
		Boost.looping = true;

		AsteroidHit = AddEffect("resources/asteroid_hit.ogg");
		Destoryed = AddEffect("resources/destoryed.ogg");
		SheldHit = AddEffect("resources/shield_hit.ogg");
		Shot = AddEffect("resources/shot.ogg");
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
			PlaySound(Effects[effect]);
	}
}