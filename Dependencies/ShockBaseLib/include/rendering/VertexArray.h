#pragma once

#include "VertexBufferLayout.h"
#include "geometry/Vertex.h"

#include "glad/glad.h"

namespace sbl
{
    class VertexArray
    {
    private:
        GLuint m_VAOID;
        std::vector<GLuint> m_VBOIDs;
        std::vector<GLuint> m_EBOIDs;   // Index buffer objects  (-> element buffer object)
    public:
        VertexArray();
        
        void Init();

        void AddVertexBuffer(GLfloat* data, int size, VertexBufferLayout& layout);
        void AddVertexBuffer(const std::vector<Vertex>& data, int size, VertexBufferLayout& layout);
        void AddIndexBuffer(GLuint* data, int size);
        void AddIndexBuffer(const std::vector<GLuint>& data, int size);

        void Bind();
        void Unbind();
        void Delete();
    };
}