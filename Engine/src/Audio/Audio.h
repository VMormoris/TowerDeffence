#pragma once
#ifndef AUDIO_H
#define AUDIO_H
#include "../Core/Core.h"
#include "../../engpch.h"

namespace Engine {
	class ENGINE_API AudioFile {
	protected:
		std::string filename;
	public:
		AudioFile(std::string filename) { this->filename = filename; }
		virtual ~AudioFile(void) {}
		virtual bool Load(void) = 0;
	};
}

#endif // !AUDIO_H