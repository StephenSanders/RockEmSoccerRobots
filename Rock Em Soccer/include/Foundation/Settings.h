/*
 * Settings.h
 * Stephen Sanders & Jake Singer
 */

#ifndef __H_GAME_SETTINGS__
#define __H_GAME_SETTINGS__

#include <engine/engine.h>
#include <engine/platform.h>
#include <string>

class Settings
{
public:
	enum {
		FULLSCREEN_DISABLED = PLATFORM_WINDOWED,
		FULLSCREEN_NORMAL   = PLATFORM_FULLSCREEN,
		FULLSCREEN_WINDOWED = PLATFORM_FULLSCREEN | PLATFORM_WINDOWED
	};

	static bool init(const std::string &path = "./config.ini");
	static bool release();

	static uint32_t getFullscreen();
	static uint32_t getWidth();
	static uint32_t getHeight();

private:
	enum {
		defaultFullscreen = true,
		defaultWidth      = 1024,
		defaultHeight     = 576
	};

	static std::string path;
};

#endif /* __H_GAME_SETTINGS__ */
