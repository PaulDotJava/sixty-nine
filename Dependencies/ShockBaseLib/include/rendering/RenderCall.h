#pragma once

#include "ShaderProgram.h"
#include "VertexArray.h"
#include "Texture.h"

namespace sbl
{
	struct RenderCall
	{
		ShaderProgram* shader;
		ShaderConfig* shaderConfig;
		VertexArray* vao;
		Texture* texture;
	};
}