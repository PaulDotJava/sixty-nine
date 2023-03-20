#pragma once

#include "ShockBaseLib/ShockBaseLib.h"

#define UPDRAFT (0.05)

class SmokeObject
{
public:
	SmokeObject(glm::vec4& color, float iScale, float aScale, float iVel, float aVel, float iRotVel, float aRotVel, float iLifeTime, float aLifeTime, float iGrothFactor, float aGrothFactor);
public:
	void Update(double dt);
	void Render(sbl::Renderer& renderer, sbl::ShaderProgram& shader, sbl::VertexArray& vao, sbl::Texture& tex);
	inline bool IsAlive() { return m_Alive; };
private:
	sbl::ShaderConfig m_ShaderConfig;
	glm::mat4 m_Transform;

	glm::vec4 m_Color;
	glm::vec3 m_Pos;

	glm::vec3 m_Velocity;	// x/y/rotation

	double m_Scale;
	double m_OrgScale;
	double m_GrothFactor;

	double m_LifeTime;
	double m_OrgLifeTime;

	bool m_Alive = true;
};