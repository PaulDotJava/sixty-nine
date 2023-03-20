#pragma once

#include <string>

#include "ShaderProgram.h"

#include "logging/Log.h"

namespace sbl
{
	class IUniform
	{
	public:
		virtual void Set(ShaderProgram& shader) = 0;
	};

	template<typename T>
	class Uniform : public IUniform
	{
	public:
		Uniform(T val, const std::string& n) {}
		void Set(ShaderProgram& shader) override
		{
			ASSERT(false, "Illegal uniform type!");
		}
	};

	template<>
	class Uniform<int> : public IUniform
	{
	public:
		Uniform(int* val, const std::string& n)
			: m_Data(val), m_Name(n) {}
		void Set(ShaderProgram& shader) override
		{
			shader.SetUniform1i(m_Name, *m_Data);
		}
	private:
		int* m_Data;
		std::string m_Name;
	};

	template<>
	class Uniform<glm::vec3> : public IUniform
	{
	public:
		Uniform(glm::vec3* val, const std::string& n)
			: m_Data(val), m_Name(n) {}
		void Set(ShaderProgram& shader) override
		{
			shader.SetUniform3f(m_Name, *m_Data);
		}
	private:
		glm::vec3* m_Data;
		std::string m_Name;
	};

	template<>
	class Uniform<glm::vec4> : public IUniform
	{
	public:
		Uniform(glm::vec4* val, const std::string& n)
			: m_Data(val), m_Name(n) {}
		void Set(ShaderProgram& shader) override
		{
			shader.SetUniform4f(m_Name, *m_Data);
		}
	private:
		glm::vec4* m_Data;
		std::string m_Name;
	};

	template<>
	class Uniform<glm::mat3> : public IUniform
	{
	public:
		Uniform(glm::mat3* val, const std::string& n)
			: m_Data(val), m_Name(n) {}
		void Set(ShaderProgram& shader) override
		{
			shader.SetUniformMat3f(m_Name, *m_Data);
		}
	private:
		glm::mat3* m_Data;
		std::string m_Name;
	};

	template<>
	class Uniform<glm::mat4> : public IUniform
	{
	public:
		Uniform(glm::mat4 *val, const std::string& n)
			: m_Data(val), m_Name(n) {}
		void Set(ShaderProgram& shader) override
		{
			shader.SetUniformMat4f(m_Name, *m_Data);
		}
	private:
		glm::mat4 *m_Data;
		std::string m_Name;
	};

}