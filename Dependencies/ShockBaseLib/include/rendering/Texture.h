#pragma once

#include "glad/glad.h"
#include <string>

namespace sbl
{
	class Texture
	{
	public:
		Texture(const std::string& path);
		Texture() = default;
		void Init(const std::string& path);
		operator GLuint() const { return m_TexID; };
		void Bind();
		void Unbind();
	private:
		GLuint m_TexID;
		uint32_t m_Width;
		uint32_t m_Height;
	};
}