// Copyright 2020 Bartosz Panus

#pragma once
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include "LightSource.h"

class PointLight : public LightSource{
 public:
  enum EColor {
    e_red,
    e_green,
    e_blue
  };

  PointLight();
  PointLight(float az_ang, float el_ang, float distance, EColor color);
  PointLight(float az_ang, float el_ang, float distance,
    GLfloat light_ambient[4], EColor color, GLfloat light_specular[4],
    GLfloat att_constant, GLfloat att_linear, GLfloat att_quadratic);
  PointLight(GLfloat light_ambient[4], EColor color, GLfloat light_specular[4],
    GLfloat att_constant, GLfloat att_linear, GLfloat att_quadratic);

  void set_angles(float az_ang, float el_ang);
  void add_angles(float az_ang, float el_ang);
  void set_dsitance(float dist);
  void add_dsitance(float dist);
  void update_pos();
  void set_light_in_scene() override;

  void set_color(EColor color);


 private:
  void normalize_angles();
  float m_az_angle, m_el_angle, m_distance;
  EColor m_color;
};

