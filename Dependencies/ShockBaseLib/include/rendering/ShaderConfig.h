#pragma once

#include <vector>

namespace sbl
{
	class ShaderProgram;
	class IUniform;

	class ShaderConfig
	{
		friend class ShaderProgram;
	public:
		ShaderConfig() = default;
		ShaderConfig(const ShaderConfig&) = default;
		~ShaderConfig();
	public:
		void AddUniform(IUniform* uni);
	private:
		std::vector<IUniform*> m_Uniforms;
	};
}