#pragma once

#include "ShockBaseLib/ShockBaseLib.h"

class Background
{
public:
	void Init();
	void RenderUpper(sbl::Renderer& renderer, sbl::ShaderProgram& shader, sbl::VertexArray& vao);
	void RenderLower(sbl::Renderer& renderer, sbl::ShaderProgram& shader, sbl::VertexArray& vao);
private:
	sbl::Texture m_Texture;
	sbl::Texture m_LowerTexture;

	sbl::ShaderConfig m_Config;
	sbl::ShaderConfig m_LowerConfig;

	glm::mat4 m_Transform;
	glm::mat4 m_LowerTransform;

	glm::vec4 m_Color;
};