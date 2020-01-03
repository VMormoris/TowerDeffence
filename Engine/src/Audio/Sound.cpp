#include "../../engpch.h"
#include "Sound.h"

namespace Engine {

	Sound::Sound(std::string filename) : AudioFile(filename) { sound = NULL; }

	Sound::~Sound(void) {
		if (sound != NULL) Mix_FreeChunk(sound);
	}

	bool Sound::Load(void) {
		sound = Mix_LoadWAV(filename.c_str());
		if (sound == NULL) {
			printf("Error while reading: %s\n", filename.c_str());
			return false;
		}
		else return true;
	}

	void Sound::Play(void) {
		if (sound == NULL) return;
		Mix_PlayChannel(-1, sound, 0);
	}
}