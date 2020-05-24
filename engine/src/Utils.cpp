#include "Utils.h"
#include "AndroidUtils.h"
#include "Log.h"
#include "glutils.h"
#include <filesystem>
#include <string_view>
#include <fstream>

namespace utils
{
	Constants& Constants::Instance()
	{
		static Constants constants;
		return constants;
	}

	Constants::Constants()
		: assetsRootPath("assets")
	{
#ifdef PLATFORM_WINDOWS
		std::string initial_path = "assets";

		for (int i = 0; i < 3; i++) {
			if (std::filesystem::exists(initial_path)) {
				assetsRootPath = initial_path;
				break;
			}
			else {
				initial_path = "../" + initial_path;
			}
		}
#endif
	}
	
	std::string Constants::GetAssetsRootPath() const
	{
		return assetsRootPath;
	}
	
	
	std::string GetFileContent(const std::string& path)
	{
		std::string filePath = path;
		// TODO: refactor
#ifdef PLATFORM_WINDOWS
		if (!std::filesystem::exists(path)) {
			std::string truePath = Constants::Instance().GetAssetsRootPath() + "/" + path;
			if (std::filesystem::exists(truePath)) {
				filePath = truePath;
			}
		}
#endif

		
		std::string str;
		if (FILE* file = fopen(filePath.c_str(), "rb")) {
			char *buffer = nullptr;
			// obtain file size:
			fseek (file , 0 , SEEK_END);
			long pos = ftell(file);
			rewind(file);

			auto nSize = static_cast<size_t>(pos);
			// allocate memory to contain the whole file:
			buffer = new char[nSize + 1];

			size_t result = fread(buffer, 1, nSize, file);
			if (result == nSize) {
				buffer[nSize] = 0;
				LOG_INFO("File successfully read: %s", filePath.c_str());
			} else {
				LOG_ERROR("File reading failed");
			}

			str = std::string(buffer);

			fclose(file);
			delete [] buffer;
		}
		else {
			LOG_ERROR("Failed to open: %s", filePath.c_str());
		}
		return str;
	}

	// https://www.khronos.org/opengl/wiki/OpenGL_Error
	void GLErrorsCheck(const char *filename, int line)
	{
		GLenum error;
		while((error = glGetError()) != GL_NO_ERROR)
		{
			std::string strError("GL error: ");

			switch (error)
			{
				case GL_INVALID_ENUM:
					strError.append("GL_INVALID_ENUM");
					break;
				case GL_INVALID_VALUE:
					strError.append("GL_INVALID_VALUE");
					break;
				case GL_INVALID_OPERATION:
					strError.append("GL_INVALID_OPERATION");
					break;
				case GL_OUT_OF_MEMORY:
					strError.append("GL_OUT_OF_MEMORY");
					break;
				case GL_INVALID_FRAMEBUFFER_OPERATION:
					strError.append("GL_INVALID_FRAMEBUFFER_OPERATION");
					break;
				default:
					strError.append(std::to_string(error));
			}

			strError.append("\n");
			LOG_ERROR("GL error: %s", strError.c_str());
		}
	}
}