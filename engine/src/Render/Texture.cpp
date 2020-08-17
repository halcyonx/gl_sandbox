#include "Texture.h"
#include <cassert>
#include <Log.h>
#include "AndroidUtils.h"
#include <stb_image.h>
#include <filesystem>
#include <Utils.h>


namespace Core
{
	Texture::~Texture()
	{
		Reset();
	}


	void Texture::Load(const std::string& filename)
	{
		LOG_INFO("Loading texture %s", filename.c_str());
		GLuint texture = 0;
		// load image, create texture and generate mipmaps
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);

		std::string filePath = filename;
		// TODO: refactor
#ifdef PLATFORM_WINDOWS
		if (!std::filesystem::exists(filename)) {
			std::string truePath = utils::Constants::Instance().GetAssetsRootPath() + "/" + filename;
			if (std::filesystem::exists(truePath)) {
				filePath = truePath;
			}
		}
#endif

		// auto content = utils::GetFileContent(filePath);

		if (unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0))
		{
			GLint internalFormat = 0; (nrChannels == 4 ? GL_RGBA : GL_RGB);
			GLenum dataFormat = 0; (nrChannels == 4 ? GL_RGBA : GL_RGB);

			if (nrChannels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (nrChannels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}

			assert(internalFormat && dataFormat);

			glGenTextures(1, &texture); glCheckError();
			glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
			glCheckError();
			// set the texture wrapping parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   // set texture wrapping to GL_REPEAT (default wrapping method)
			glCheckError();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glCheckError();
			// set texture filtering parameters
			// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glCheckError();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glCheckError();
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
			glCheckError();
			glGenerateMipmap(GL_TEXTURE_2D);
			glCheckError();
			glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
			glCheckError();

			stbi_image_free(data);
			LOG_INFO("Texture loaded %s; (%d x %d:%d)", filename.c_str(), width, height, nrChannels);
		}
		else
		{
			LOG_ERROR("Texture loading failed %s, %d %d", filename.c_str(), width, height);
		}

		_texture = texture;
	}

	void Texture::Reset()
	{
		glDeleteTextures(1, &_texture);
		glCheckError();
	}
}
