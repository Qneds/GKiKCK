// Copyright 2020 Bartosz Panus

#pragma once
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
class MyMouse {
 public:
  MyMouse();

  enum EState {
    e_null = 0,
    e_left_button = 1,
    e_right_button = 2,
    e_scroll_up = 4,
    e_scroll_down = 8
  };

  void update(int button, int state, int x, int y);
  void update_pos(int x, int y);
  int get_delta_x();
  int get_delta_y();
  int get_state();

 private:
  int m_state = e_null;
  int m_x_old_pos, m_y_old_pos;
  int m_x_pos, m_y_pos;
  int m_delta_x, m_delta_y;
};

