#pragma once
#ifdef ENGINE_BUILD_DLL
	/**
	* Definition export or import
	*/
	#define ENGINE_API __declspec(dllexport)
#else
	/**
	* Definition export or import
	*/
	#define ENGINE_API __declspec(dllimport)
#endif
