#pragma once
#include "BufferOpenGL.h"

namespace GL {

      class ShaderStorageBuffer;
      using ShaderStorageBufferPtr = std::shared_ptr<ShaderStorageBuffer>;

      // буфер, к которому из шейдера можно обращаться как к массиву
      class ShaderStorageBuffer : public BufferOpenGL
      {
        int m_nLayout = 0; // привязка к переменной в шейдере

      public:
        ShaderStorageBuffer(int);

      public:

        void bound() const override;
        void unbound() const override;
      };
}