#pragma once

#include <stdint.h>
#include <string>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "rendering/Renderer.h"

namespace sbl
{
	class Window
	{
	public:
		void Init(uint16_t width, uint16_t height, const std::string& title);
		void Run();
	private:
		virtual void Update(double dt) {};
		virtual void Render() {};
		virtual void ImGuiRender() {};

		virtual void Start() {}

		virtual void Delete() {};
	protected:
		uint16_t m_Width;
		uint16_t m_Height;
		std::string m_Title;
		std::unique_ptr<Renderer> m_Renderer;
	private:
		GLFWwindow* m_WindowHandle;
	};
}