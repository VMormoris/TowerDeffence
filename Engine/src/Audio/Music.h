#pragma once
#ifndef MUSIC_H
#define MUSIC_H
#include "Audio.h"

namespace Engine {

	class ENGINE_API Music : public AudioFile {
	private:
		Mix_Music* music;
	public:
		Music(std::string filename);
		~Music(void);
		bool Load(void) override;
		void Play(void);
		void Pause(void);
		void Resume(void);
	};

}
#endif // !MUSIC_H
