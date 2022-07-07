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
      virtual void draw() const = 0;
      virtual void sizeChanged(int nWidth_, int nHeight_) const = 0;

    public:
      virtual void on_handle_created() {}
      virtual void on_handle_changed() {}
      virtual void on_handle_destroyed() {}

      virtual void on_mouse_scroll(float fScroll_) {}
      virtual void on_mouse_double_click() {}
      virtual lib::fPoint2D on_mouse_click(int nPosX_, int nPosY_) { return lib::fPoint2D(9999, 9999); }
      virtual void on_mouse_left_btn_move(int nMoveX_, int nMoveY_) {}
      virtual void on_mouse_right_btn_move(int nMoveX_, int nMoveY_) {}
    };
}