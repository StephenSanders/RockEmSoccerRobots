#include <engine/types.h>
#include <windows.h>

float time_delta, time_current;
double _time_current, _frequency, _time_next;

void time_init(void)
{
	LARGE_INTEGER data;
	if (QueryPerformanceFrequency(&data)) {
		_frequency = (double)(data.QuadPart);
	} else {
		_frequency = 0;
	}
	QueryPerformanceCounter(&data);
	_time_current = (double)(data.QuadPart)/_frequency;
	time_current = (float)_time_current;
	time_delta = (1.0f / 60.0f);
	return;
}

void time_update(void)
{
	LARGE_INTEGER data;
	QueryPerformanceCounter(&data);
	_time_next = (double)(data.QuadPart)/_frequency;
	time_delta = (float)(_time_next - _time_current);
	_time_current = _time_next;
	time_current = (float)_time_current;
}
