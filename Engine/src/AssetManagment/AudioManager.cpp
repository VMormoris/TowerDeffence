#include "../../engpch.h"
#include "AudioManager.h"

namespace Engine {


	std::vector<Sound*> AudioManager::sounds;
	std::vector<std::string> AudioManager::sound_names;
	std::vector<Music*> AudioManager::musics;
	std::vector<std::string> AudioManager::music_names;



	void AudioManager::Init(void) {
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
			printf("Error while initiliazing Audio.\n\t%s\n", Mix_GetError());
		}
	}

	void AudioManager::Clear(void) {

		Mix_Quit();
		for (Music* music : musics)
			delete music;
		for (Sound* sound : sounds)
			delete sound;

	}

	int AudioManager::findSound(const char* filename) {
		for (int i = 0; i < sound_names.size();i++) {
			if (sound_names[i].compare(filename) == 0)return i;
		}
		return -1;
	}

	int AudioManager::findMusic(const char* filename) {
		for (int i = 0; i < music_names.size(); i++) {
			if (music_names[i].compare(filename) == 0) return i;
		}
		return -1;
	}


	Sound* AudioManager::RequestSound(const char* filename) {
		int index = findSound(filename);
		if (index != -1) return sounds[index];
		std::string new_soundname(filename);
		Sound* new_sound = new Sound(new_soundname);
		if (new_sound->Load()) {
			sounds.push_back(new_sound);
			sound_names.push_back(new_soundname);
		}
		else return NULL;
		return new_sound;
	}

	Music* AudioManager::RequestMusic(const char* filename) {
		int index = findMusic(filename);
		if (index != 1) return musics[index];
		std::string new_musicname(filename);
		Music* new_music = new Music(new_musicname);
		if (new_music->Load()) {
			musics.push_back(new_music);
			music_names.push_back(new_musicname);
		}
	}

}