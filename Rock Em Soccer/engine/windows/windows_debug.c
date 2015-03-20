/*
 * <engine/debug.h>
 * Stephen Sanders & Jake Singer
 */

#include <engine/debug.h>
#include <engine/define.h>

#if WINDOWS

#include <stdio.h>
#include <windows.h>

void debug_print(const char *message)
{
	OutputDebugString(message);
	return;
}

#endif /* WINDOWS */