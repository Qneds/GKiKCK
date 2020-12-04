// Copyright 2020 Bartosz Panus

#pragma once
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include "LightSource.h"

class DirectionalLight : public LightSource {
 public:
  DirectionalLight();
  explicit DirectionalLight(GLfloat pos[3]);
  DirectionalLight(GLfloat pos[3], GLfloat light_ambient[4],
    GLfloat light_diffuse[4], GLfloat light_specular[4],
    GLfloat att_constant, GLfloat att_linear, GLfloat att_quadratic);
  DirectionalLight(GLfloat light_ambient[4], GLfloat light_diffuse[4],
    GLfloat light_specular[4], GLfloat att_constant,
    GLfloat att_linear, GLfloat att_quadratic);
  void spin();
  void set_light_in_scene() override;

 private:
  void normalize_angles();
  GLfloat m_point[3], m_theta{ 0.f };
};

