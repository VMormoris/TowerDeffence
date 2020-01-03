#include "../../engpch.h"
#include "Music.h"

namespace Engine {

	Music::Music(std::string filename) : AudioFile(filename) { music = NULL; }

	Music::~Music(void) {
		if (music != NULL) Mix_FreeMusic(music);
	}

	bool Music::Load(void) {
		music = Mix_LoadMUS(filename.c_str());
		if (music == NULL) {
			printf("Error while reading: %s\n", filename.c_str());
			return false;
		}
		else return true;
	}


	void Music::Play(void) {
		if (music == NULL) return;
		Mix_PlayMusic(music, -1);
	}

	void Music::Pause(void) {
		if (music == NULL) return;
		if (Mix_PlayingMusic() != 0) Mix_PauseMusic();
	}

	void Music::Resume(void) {
		if (music == NULL) return;
		if (Mix_PausedMusic() != 0) Mix_ResumeMusic();
	}
}