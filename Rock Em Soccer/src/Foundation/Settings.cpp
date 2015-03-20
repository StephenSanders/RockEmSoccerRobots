/*
 * Settings.cpp
 * Stephen Sanders & Jake Singer
 */

#include <Foundation/Settings.h>
#include <stdlib.h>
#include <fstream>

#if WINDOWS
#include <windows.h>
#endif

std::string Settings::path;

bool Settings::init(const std::string &path)
{
	std::fstream stream;
	stream.open(path, std::fstream::in);
	if (!stream.is_open()) {
		/* create new ini file */
		stream.clear();
		stream.open(path, std::fstream::out);
		if (!stream.is_open()) {
			/* file creation failed */
			stream.close();
			return false;
		}

		/* write default ini file */
		stream <<
			"[Display]" <<
			"\nFullscreen=" << defaultFullscreen <<
			"\nWidth=" << defaultWidth <<
			"\nHeight=" << defaultHeight <<
			"\n";
	}
	stream.close();
	Settings::path = path;
	return true;
}

bool Settings::release()
{
	return true;
}

uint32_t Settings::getFullscreen()
{
	TCHAR buffer[64];
	if (GetPrivateProfileString("Display", "Fullscreen", NULL, buffer, 64, path.c_str())) {
		switch (atoi(buffer)) {
			case 0:
				return FULLSCREEN_DISABLED;
			case 1:
				return FULLSCREEN_NORMAL;
			case 2:
				return FULLSCREEN_WINDOWED; 
			default:
				break;
		}
	}
	return defaultFullscreen;
}

uint32_t Settings::getWidth()
{
	TCHAR buffer[64];
	if (GetPrivateProfileString("Display", "Width", NULL, buffer, 64, path.c_str())) {
		return atoi(buffer);
	}
	return defaultWidth;
}

uint32_t Settings::getHeight()
{
	TCHAR buffer[64];
	if (GetPrivateProfileString("Display", "Height", NULL, buffer, 64, path.c_str())) {
		return atoi(buffer);
	}
	return defaultHeight;
}
