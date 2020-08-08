#include "Timer.h"

#include <cassert>
#include <chrono>

#ifdef PLATFORM_WINDOWS
#include <GLFW/glfw3.h>
#endif

namespace
{
	double startTimePoint = 0.0;
}

void utils::initTime()
{
	static bool init = false;
	assert(!init);
	if (!init)
	{
		using namespace std::chrono;
		startTimePoint = static_cast<double>(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
		init = true;
	}
}

float utils::getTime()
{
#ifdef PLATFORM_WINDOWS
	return glfwGetTime();
#elif PLATFORM_ANDROID
	using namespace std::chrono;
	auto now = static_cast<double>(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
	auto ms = (now - startTimePoint) * 0.001;
	return ms;
#endif
}
