#pragma once

#include "Renderer.h"

namespace mars
{
	class RendererOpenGL : public Renderer
	{
	public:
		RendererOpenGL();
		void Draw() override;
		void Step() override;
		void Resize(int width, int height) override;
		void Render() override;
	};
}
