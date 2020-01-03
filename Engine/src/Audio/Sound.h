#pragma once
#ifndef SOUND_H
#define SOUND_H

#include "Audio.h"

namespace Engine {

	class ENGINE_API Sound : public AudioFile {
	private:
		Mix_Chunk* sound;
	public:
		Sound(std::string filename);
		~Sound(void);
		bool Load(void) override;
		void Play(void);
	};

}
#endif // !SOUND_H
