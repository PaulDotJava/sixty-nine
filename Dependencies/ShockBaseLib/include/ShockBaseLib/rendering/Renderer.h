#pragma once

#include "glad/glad.h"
#include "ShaderProgram.h"
#include "RenderCall.h"

#include <deque>

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearErrors();\
x;\
ASSERT(GLLogCall(#x, __FILE__, __LINE__))

namespace sbl
{
	void GLClearErrors();

	bool GLLogCall(const char* function, const char* file, int line);

	class Renderer
	{
	public:
		static void Render();
		static void QueueCall(const RenderCall& call);
	private:
		static std::deque<RenderCall> m_RenderQueue;
	};
}