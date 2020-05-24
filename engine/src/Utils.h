#pragma once

#include <string>


#ifdef DEBUG
#define GL_CHECK_ERRORS utils::GLErrorsCheck(__FILE__, __LINE__);
#else
#define GL_CHECK_ERRORS
#endif

namespace utils
{
    std::string GetFileContent(const std::string& path);
    void GLErrorsCheck(const char *filename, int line);

	class Constants
	{
	public:
		static Constants& Instance();
		std::string GetAssetsRootPath() const;
	private:
		Constants();
		
		std::string assetsRootPath;
	};
}