#pragma once

// File to be included by the client
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Window.h"
#include "rendering/VertexArray.h"
#include "rendering/ShaderProgram.h"
#include "rendering/ShaderConfig.h"
#include "rendering/Uniform.h"
#include "rendering/Renderer.h"
#include "rendering/Texture.h"