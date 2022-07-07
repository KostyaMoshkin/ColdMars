#pragma once
#include "BufferOpenGL.h"

namespace GL {

      class ShaderStorageBuffer;
      using ShaderStorageBufferPtr = std::shared_ptr<ShaderStorageBuffer>;

      // �����, � �������� �� ������� ����� ���������� ��� � �������
      class ShaderStorageBuffer : public BufferOpenGL
      {
        int m_nLayout = 0; // �������� � ���������� � �������

      public:
        ShaderStorageBuffer(int);

      public:

        void bound() const override;
        void unbound() const override;
      };
}