#include "pch.h"

#include "IndexBuffer.h"

namespace GL {

      IndexBuffer::IndexBuffer(int size_)
        : BufferOpenGL(GL_ELEMENT_ARRAY_BUFFER, size_, GL_STATIC_DRAW)
      {
      }

      IndexBuffer::IndexBuffer()
        : BufferOpenGL(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW)
      {

      }
}