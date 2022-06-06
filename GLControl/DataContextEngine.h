#pragma once

namespace GL {

    class DataContextEngine
    {
    public:
      DataContextEngine()
      {
      }

      virtual ~DataContextEngine()
      {
      }

    public:
      virtual void draw(bool bForce_ = false) = 0;

    public:
      virtual void on_handle_created() {}

      virtual void on_handle_changed() {};

      virtual void on_handle_destroyed() {}
    };
}