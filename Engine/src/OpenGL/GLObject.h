#pragma once
#ifndef GL_OBJECT_H
#define GL_OBJECT_H
#include "../Core/Core.h"

namespace Engine{

	class ENGINE_API GLObject {
	protected:

		/**
		* ID used for describing the Object
		*/
		unsigned int m_ID;

	public:

		/**
		* Used for binding Object
		*/
		virtual void Bind(void) const = 0;
		/**
		* Used for unbinding Object
		*/
		virtual void Unbind(void) const = 0;

		/**
		* Used for generating Object
		*/
		virtual void Generate(void) = 0;
	};

}


#endif//!GL_OBJECT_H