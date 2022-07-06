#include "pch.h"

#include "ShaderStorageBuffer.h"


namespace GL {

      ShaderStorageBuffer::ShaderStorageBuffer(int layout_)
        : BufferOpenGL(GL_SHADER_STORAGE_BUFFER, GL_STATIC_DRAW)
      {
        m_nLayout = layout_;
      }

      void ShaderStorageBuffer::bound() const
      {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_nLayout, m_nBufferId);
      }

      void ShaderStorageBuffer::unbound() const
      {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
      }
}