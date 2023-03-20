#include "Background.h"

void Background::Init()
{
	m_Transform = glm::mat4(1.0f);
	m_LowerTransform = glm::mat4(1.0f);

	m_Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	m_Texture.Init("./res/textures/BackgroundAd.png");
	m_LowerTexture.Init("./res/textures/BackgroundAdLower.png");

	m_Transform = glm::translate(m_Transform, glm::vec3(800.0f, 400.0f, 0.0f));
	m_Transform = glm::scale(m_Transform, glm::vec3(1600.0f, 800.0f, 1.0f));

	m_LowerTransform = glm::translate(m_LowerTransform, glm::vec3(1140.0f, 237.0f, 0.0f));
	m_LowerTransform = glm::scale(m_LowerTransform, glm::vec3(570.0f, 266.0f, 1.0f));

	m_Config.AddUniform(new sbl::Uniform<glm::mat4>(&m_Transform, "u_ModelMatrix"));
	m_Config.AddUniform(new sbl::Uniform<glm::vec4>(&m_Color, "u_Color"));

	m_LowerConfig.AddUniform(new sbl::Uniform<glm::mat4>(&m_LowerTransform, "u_ModelMatrix"));
	m_LowerConfig.AddUniform(new sbl::Uniform<glm::vec4>(&m_Color, "u_Color"));
}

void Background::RenderUpper(sbl::Renderer& renderer, sbl::ShaderProgram& shader, sbl::VertexArray& vao)
{
	renderer.QueueCall({ &shader, &m_Config, &vao, &m_Texture });
}

void Background::RenderLower(sbl::Renderer& renderer, sbl::ShaderProgram& shader, sbl::VertexArray& vao)
{
	renderer.QueueCall({ &shader, &m_LowerConfig, &vao, &m_LowerTexture });
}