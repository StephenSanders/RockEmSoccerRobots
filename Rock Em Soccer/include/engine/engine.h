/*
 * <engine/engine.h>
 * Stephen Sanders & Jake Singer
 *
 * Include this file for all common engine headers.
 *
 * This must be the first include in the main file in order to initialize the
 * engine properly. It is encouraged that specific engine header files be
 * included rather than this file elsewhere in the game to lower compile time
 * (though this is an acceptable include anywhere during early development).
 *
 * The game engine is written in C with the intention of maintaining a simple
 * API with minimal functionality. C and C++ are both fully supported as
 * calling languages. Higher levels of logic and functionality may be added
 * within the game code.
 */

#ifndef __H_ENGINE__
#define __H_ENGINE__

#include <engine/debug.h>
#include <engine/define.h>
#include <engine/memory.h>
#include <engine/time.h>
#include <engine/types.h>

#endif /* __H_ENGINE__ */
