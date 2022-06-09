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
      virtual void draw() = 0;

    public:
      virtual void on_handle_created() {}
      virtual void on_handle_changed() {}
      virtual void on_handle_destroyed() {}

      virtual void on_mouse_scroll(float fScroll_) {}
      virtual void on_mouse_double_click() {}
      virtual void on_mouse_left_btn_move(int nMoveX_, int nMoveY_) {}
      virtual void on_mouse_right_btn_move(int nMoveX_, int nMoveY_) {}
    };
}