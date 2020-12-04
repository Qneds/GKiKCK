// Copyright 2020 Bartosz Panus

#pragma once
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>

class Material {
 public:
  Material();
  Material(GLfloat ambient[4], GLfloat diffuse[4], GLfloat specular[4],
           GLfloat shiness);

  void set_up_material();
  void set_ambient(GLfloat ambient[4]);
  void set_diffuse(GLfloat difuse[4]);
  void set_specular(GLfloat specular[4]);
  void set_shiness(GLfloat shiness);

 private:
  GLfloat ambient[4]{1.0, 1.0, 1.0, 1.0};
  GLfloat diffuse[4]{1.0, 1.0, 1.0, 1.0};
  GLfloat specular[4]{1.0, 1.0, 1.0, 1.0};
  GLfloat shiness{20.0};
};
