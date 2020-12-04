// Copyright 2020 Bartosz Panus

#pragma once
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>

class LightSource {
 public:
  LightSource();
  ~LightSource();
  virtual void set_light_in_scene() = 0;
  void enable_light() { m_b_light_enabled = true; glEnable(m_light_id); }
  void disable_light() { m_b_light_enabled = false; glDisable(m_light_id); }
  bool try_create();
  size_t get_id() { return m_light_id; }
  bool is_enabled() { return m_b_light_enabled; }
  bool is_created() { return m_b_is_created; }

 protected:
  GLfloat m_light_ambient[4] {};
  GLfloat m_light_diffuse[4] {};
  GLfloat m_light_specular[4] {};
  GLfloat m_light_position[4] {};
  GLfloat m_att_constant {}, m_att_linear{}, m_att_quadratic{};

 private:
  static bool m_ba_is_index_used[10];
  static size_t m_used_indexes;
  size_t m_light_id;
  bool m_b_light_enabled;
  bool m_b_is_created;
  void create();
};
