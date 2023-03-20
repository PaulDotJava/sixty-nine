#include "SmokeObject.h"

#include <random>

SmokeObject::SmokeObject(glm::vec4& color, float iScale, float aScale, float iVel, float aVel, float iRotVel, float aRotVel, float iLifeTime, float aLifeTime, float iGrothFactor, float aGrothFactor)
{
	m_Color = color;

	m_ShaderConfig.AddUniform(new sbl::Uniform<glm::vec4>(&m_Color, "u_Color"));
	m_ShaderConfig.AddUniform(new sbl::Uniform<glm::mat4>(&m_Transform, "u_ModelMatrix"));

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> posDist(iVel, aVel);
	std::uniform_int_distribution<> rotDist(iRotVel, aRotVel);
	std::uniform_int_distribution<> lifeDist(iLifeTime, aLifeTime);
	std::uniform_int_distribution<> scaleDist(iScale, aScale);

	m_Velocity = glm::vec3(posDist(gen), posDist(gen), rotDist(gen));
	m_OrgLifeTime = lifeDist(gen);
	m_LifeTime = m_OrgLifeTime;
	m_Scale = scaleDist(gen);
	m_OrgScale = m_Scale;

	m_Pos = glm::vec3(1125.0f, 428.0f, 0.0f);
}

void SmokeObject::Update(double dt)
{
	double ageFactor = m_LifeTime / m_OrgLifeTime;

	m_Color.a = ageFactor;

	m_Pos.x += m_Velocity.x * dt * ageFactor;
	m_Pos.y += (m_Velocity.y * dt * ageFactor) + UPDRAFT;
	m_Pos.z += m_Velocity.z * dt * ageFactor;

	//m_Scale *= (1 - ageFactor);

	m_Transform = glm::mat4(1.0f);
	m_Transform = glm::translate(m_Transform, glm::vec3(m_Pos.x, m_Pos.y, 1.0f));
	m_Transform = glm::rotate(m_Transform, glm::radians(m_Pos.z), glm::vec3(0.0f, 0.0f, 1.0f));
	m_Transform = glm::scale(m_Transform, glm::vec3(m_Scale, m_Scale, 1.0f));

	m_LifeTime -= dt;

	if (m_LifeTime <= 0)
	{
		m_Alive = false;
	}
}

void SmokeObject::Render(sbl::Renderer& renderer, sbl::ShaderProgram& shader, sbl::VertexArray& vao, sbl::Texture& tex)
{
	renderer.QueueCall({ &shader, &m_ShaderConfig, &vao, &tex });
}