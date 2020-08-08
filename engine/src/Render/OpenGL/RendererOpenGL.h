#pragma once

#include "Renderer.h"

namespace mars
{
	class RendererOpenGL : public Renderer
	{
	public:
		RendererOpenGL();
		float Step() override;
		void Resize(int width, int height) override;
		void Render() override;
	};
}
