#pragma once

#include <cstdlib>

#ifdef _WIN32
# include <windows.h>
# define seed() ::GetCurrentTime()
#else
# include <ctime>
# define seed() std::time(NULL)
#endif


class Random
{
public:

	Random(void)
	{
		srand(static_cast<UINT>(seed()));
	}

	UINT operator()(UINT max)
	{
		double tmp = static_cast<double>( rand()) / static_cast<double>(RAND_MAX);
		return static_cast<UINT>(tmp * max);
	}

	~Random(void){}
};
