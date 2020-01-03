#pragma once
#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include "../Audio/Music.h"
#include "../Audio/Sound.h"

namespace Engine {

	class ENGINE_API AudioManager {
	protected:

		static std::vector<Sound*> sounds;
		static std::vector<std::string> sound_names;
		static std::vector<Music*> musics;
		static std::vector<std::string> music_names;

		static int findSound(const char* filename);
		static int findMusic(const char* filename);

	public:

		static void Init(void);
		static Sound* RequestSound(const char* filename);
		static Music* RequestMusic(const char* filename);
		static void Clear(void);

	};
}
#endif // !AUDIO_MANAGER_H
