// Copyright 2020 Bartosz Panus

#include "Material.h"

Material::Material() {}

Material::Material(GLfloat ambient[4], GLfloat diffuse[4], GLfloat specular[4],
                   GLfloat shiness) {
  this->ambient[0] = ambient[0];
  this->ambient[1] = ambient[1];
  this->ambient[2] = ambient[2];
  this->ambient[3] = ambient[3];

  this->diffuse[0] = diffuse[0];
  this->diffuse[1] = diffuse[1];
  this->diffuse[2] = diffuse[2];
  this->diffuse[3] = diffuse[3];

  this->specular[0] = specular[0];
  this->specular[1] = specular[1];
  this->specular[2] = specular[2];
  this->specular[3] = specular[3];

  this->shiness = shiness;
}

void Material::set_up_material() {
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialf(GL_FRONT, GL_SHININESS, shiness);
}

void Material::set_ambient(GLfloat ambient[4]) {
  this->ambient[0] = ambient[0];
  this->ambient[1] = ambient[1];
  this->ambient[2] = ambient[2];
  this->ambient[3] = ambient[3];
}

void Material::set_diffuse(GLfloat diffuse[4]) {
  this->diffuse[0] = diffuse[0];
  this->diffuse[1] = diffuse[1];
  this->diffuse[2] = diffuse[2];
  this->diffuse[3] = diffuse[3];
}

void Material::set_specular(GLfloat specular[4]) {
  this->specular[0] = specular[0];
  this->specular[1] = specular[1];
  this->specular[2] = specular[2];
  this->specular[3] = specular[3];
}

void Material::set_shiness(GLfloat shiness) { this->shiness = shiness; }
