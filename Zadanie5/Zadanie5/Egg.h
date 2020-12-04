// Copyright 2020 Bartosz Panus

#pragma once
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include "Texture.h"
#include "Material.h"

class Egg {
 public:
  typedef float point3d[3];
  Egg(unsigned N, const point3d cords, float scale);
  ~Egg();



  void material();
  void texture();
  void draw();
  void spin_egg();
  void set_spin_theta(bool x, bool y, bool z);
  void set_texture(Texture text);

 private:
  point3d** m_points;
  point3d** m_normals;
  point3d m_cords;
  point3d m_spin{0.0, 0.0, 0.0};
  Material m_material;
  Texture m_texture;

  unsigned m_N;
  float m_dist;
  float m_scale_val;

  bool m_spinning[3]{};

  float inc_f(float in);

  float fxu(float u, float v);
  float fxv(float u, float v);
  float fyu(float u, float v);
  float fyv(float u, float v);
  float fzu(float u, float v);
  float fzv(float u, float v);
  float length(point3d** arr, int i, int j);
  void generate_points();
  float rand_color();
};
