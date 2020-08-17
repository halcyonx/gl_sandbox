#pragma once

#include <glutils.h>
#include <string>

namespace  Core
{
	class Texture
	{
	public:
		Texture() = default;
		~Texture();
		void Load(const std::string& filename);
		void Reset();
		GLuint  GetHandle() const { return _texture; }

	private:
		GLuint _texture = 0;
	};
}
