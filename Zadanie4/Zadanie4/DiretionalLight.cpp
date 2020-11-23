// Copyright 2020 Bartosz Panus

#define _USE_MATH_DEFINES
#include "DirectionalLight.h"
#include <math.h>

DirectionalLight::DirectionalLight() {
    m_light_position[0] = 15.f;
    m_light_position[1] = 0.f;
    m_light_position[2] = 0.f;
    m_light_position[3] = 0.f;

    glLightfv(get_id(), GL_AMBIENT, m_light_ambient);
    glLightfv(get_id(), GL_DIFFUSE, m_light_diffuse);
    glLightfv(get_id(), GL_SPECULAR, m_light_specular);
    glLightf(get_id(), GL_CONSTANT_ATTENUATION, m_att_constant);
    glLightf(get_id(), GL_LINEAR_ATTENUATION, m_att_linear);
    glLightf(get_id(), GL_QUADRATIC_ATTENUATION, m_att_quadratic);
}

DirectionalLight::DirectionalLight(GLfloat pos[3]) {
  m_light_position[0] = pos[0];
  m_light_position[1] = pos[1];
  m_light_position[2] = pos[2];
  m_light_position[3] = 0.f;

  glLightfv(get_id(), GL_AMBIENT, m_light_ambient);
  glLightfv(get_id(), GL_DIFFUSE, m_light_diffuse);
  glLightfv(get_id(), GL_SPECULAR, m_light_specular);
  glLightf(get_id(), GL_CONSTANT_ATTENUATION, m_att_constant);
  glLightf(get_id(), GL_LINEAR_ATTENUATION, m_att_linear);
  glLightf(get_id(), GL_QUADRATIC_ATTENUATION, m_att_quadratic);
}

DirectionalLight::DirectionalLight(GLfloat pos[3], GLfloat light_ambient[4],
  GLfloat light_diffuse[4], GLfloat light_specular[4], GLfloat att_constant,
  GLfloat att_linear, GLfloat att_quadratic) {
    m_att_linear = att_linear;
    m_att_constant = att_constant;
    m_att_quadratic = att_quadratic;

    m_light_position[0] = pos[0];
    m_light_position[1] = pos[1];
    m_light_position[2] = pos[2];
    m_light_position[3] = 0.f;

    m_light_ambient[0] = light_ambient[0];
    m_light_ambient[1] = light_ambient[1];
    m_light_ambient[2] = light_ambient[2];
    m_light_ambient[3] = light_ambient[3];

    m_light_diffuse[0] = light_diffuse[0];
    m_light_diffuse[1] = light_diffuse[1];
    m_light_diffuse[2] = light_diffuse[2];
    m_light_diffuse[3] = light_diffuse[3];

    m_light_specular[0] = light_specular[0];
    m_light_specular[1] = light_specular[1];
    m_light_specular[2] = light_specular[2];
    m_light_specular[3] = light_specular[3];

    glLightfv(get_id(), GL_AMBIENT, m_light_ambient);
    glLightfv(get_id(), GL_DIFFUSE, m_light_diffuse);
    glLightfv(get_id(), GL_SPECULAR, m_light_specular);
    glLightf(get_id(), GL_CONSTANT_ATTENUATION, m_att_constant);
    glLightf(get_id(), GL_LINEAR_ATTENUATION, m_att_linear);
    glLightf(get_id(), GL_QUADRATIC_ATTENUATION, m_att_quadratic);
}

DirectionalLight::DirectionalLight(GLfloat light_ambient[4], GLfloat
  light_diffuse[4], GLfloat light_specular[4], GLfloat att_constant,
  GLfloat att_linear, GLfloat att_quadratic) {
    m_att_linear = att_linear;
    m_att_constant = att_constant;
    m_att_quadratic = att_quadratic;

    m_light_position[0] = 15.f;
    m_light_position[1] = 0.f;
    m_light_position[2] = 0.f;
    m_light_position[3] = 0.f;

    m_light_ambient[0] = light_ambient[0];
    m_light_ambient[1] = light_ambient[1];
    m_light_ambient[2] = light_ambient[2];
    m_light_ambient[3] = light_ambient[3];

    m_light_diffuse[0] = light_diffuse[0];
    m_light_diffuse[1] = light_diffuse[1];
    m_light_diffuse[2] = light_diffuse[2];
    m_light_diffuse[3] = light_diffuse[3];

    m_light_specular[0] = light_specular[0];
    m_light_specular[1] = light_specular[1];
    m_light_specular[2] = light_specular[2];
    m_light_specular[3] = light_specular[3];

    glLightfv(get_id(), GL_AMBIENT, m_light_ambient);
    glLightfv(get_id(), GL_DIFFUSE, m_light_diffuse);
    glLightfv(get_id(), GL_SPECULAR, m_light_specular);
    glLightf(get_id(), GL_CONSTANT_ATTENUATION, m_att_constant);
    glLightf(get_id(), GL_LINEAR_ATTENUATION, m_att_linear);
    glLightf(get_id(), GL_QUADRATIC_ATTENUATION, m_att_quadratic);
}

// obrot swiatla
void DirectionalLight::spin() {
  m_theta += 1.f;
  if (m_theta > 360.0) m_theta -= 360.0;
}

// ustawienie zrodla podczas renderowania
void DirectionalLight::set_light_in_scene() {
  if (is_enabled()) {
    glPushMatrix();
    glRotatef(m_theta, 0.f, 1.f, 0.f);
    glLightfv(get_id(), GL_POSITION, m_light_position);
    glPopMatrix();
  }
}

// ustawienie katow na wartosci z przedzialu  -pi - pi
void DirectionalLight::normalize_angles() {
  if (m_theta > M_PI)
    m_theta -= 2 * M_PI;
  if (m_theta <= -M_PI)
    m_theta += 2 * M_PI;
}
