// Copyright 2020 Bartosz Panus

#define _USE_MATH_DEFINES
#include "PointLight.h"
#include <math.h>

PointLight::PointLight() : PointLight(0.f, 0.f, 15.f, EColor::e_white) {
}

PointLight::PointLight(float az_ang, float el_ang,
  float distance, EColor color) :
  m_az_angle{ az_ang },
  m_el_angle{ el_ang },
  m_distance{ distance },
  m_color{ color } {
    m_light_diffuse[3] = 1.f;
    set_color(color);

    glLightfv(get_id(), GL_AMBIENT, m_light_ambient);
    glLightfv(get_id(), GL_SPECULAR, m_light_specular);
    glLightf(get_id(), GL_CONSTANT_ATTENUATION, m_att_constant);
    glLightf(get_id(), GL_LINEAR_ATTENUATION, m_att_linear);
    glLightf(get_id(), GL_QUADRATIC_ATTENUATION, m_att_quadratic);

    normalize_angles();
    update_pos();
}

PointLight::PointLight(float az_ang, float el_ang, float distance,
  GLfloat light_ambient[4], EColor color, GLfloat light_specular[4],
  GLfloat att_constant, GLfloat att_linear,  GLfloat att_quadratic) :
    m_az_angle{ az_ang },
    m_el_angle{ el_ang },
    m_distance{ distance },
    m_color{ color } {
      m_att_linear = att_linear;
      m_att_constant = att_constant;
      m_att_quadratic = att_quadratic;

      m_light_position[3] = 1.f;

      m_light_ambient[0] = light_ambient[0];
      m_light_ambient[1] = light_ambient[1];
      m_light_ambient[2] = light_ambient[2];
      m_light_ambient[3] = light_ambient[3];

      m_light_diffuse[3] = 1.f;
      set_color(color);

      m_light_specular[0] = light_specular[0];
      m_light_specular[1] = light_specular[1];
      m_light_specular[2] = light_specular[2];
      m_light_specular[3] = light_specular[3];

      glLightfv(get_id(), GL_AMBIENT, m_light_ambient);
      glLightfv(get_id(), GL_SPECULAR, m_light_specular);
      glLightf(get_id(), GL_CONSTANT_ATTENUATION, m_att_constant);
      glLightf(get_id(), GL_LINEAR_ATTENUATION, m_att_linear);
      glLightf(get_id(), GL_QUADRATIC_ATTENUATION, m_att_quadratic);

      normalize_angles();
      update_pos();
}

PointLight::PointLight(GLfloat light_ambient[4], EColor color,
  GLfloat light_specular[4], GLfloat att_constant,
  GLfloat att_linear, GLfloat att_quadratic) :
    PointLight(0.f, 0.f, 15.f, light_ambient, color, light_specular,
      att_constant, att_linear, att_quadratic) { }

// ustawienie katow azymutu i elewacji
void PointLight::set_angles(float az_ang, float el_ang) {
  m_az_angle = az_ang;
  m_el_angle = el_ang;
  normalize_angles();

  update_pos();
}

// dodanie wartosci katow azymutu i elewacji
void PointLight::add_angles(float az_ang, float el_ang) {
  m_az_angle += az_ang;
  m_el_angle += el_ang;

  normalize_angles();
  update_pos();
}

// ustawienie dystansu zrodla swiatla od punktu (0,0,0)
void PointLight::set_dsitance(float dist) {
  m_distance = dist;
  update_pos();
}

// zwiekszenie dystansu o dana wartosc
void PointLight::add_dsitance(float dist) {
  m_distance += dist;
  update_pos();
}

// obliczanie pozycji zrodla swiatla na bazie katow i dystansu
void PointLight::update_pos() {
  m_light_position[0] = m_distance * cos(m_az_angle) * cos(m_el_angle);
  m_light_position[1] = m_distance * sin(m_el_angle);
  m_light_position[2] = m_distance * sin(m_az_angle) * cos(m_el_angle);
}

// ustawienie zrodla podczas renderowania
void PointLight::set_light_in_scene() {
  if (is_enabled()) {
    glLightfv(get_id(), GL_POSITION, m_light_position);
  }
}

// ustawienie koloru swiatla
void PointLight::set_color(EColor color) {
  m_light_diffuse[0] = 0.f;
  m_light_diffuse[1] = 0.f;
  m_light_diffuse[2] = 0.f;

  m_color = color;

  switch (m_color) {
  case EColor::e_blue:
      m_light_diffuse[2] = 1.f;
      break;
  case EColor::e_green:
      m_light_diffuse[1] = 1.f;
      break;
  case EColor::e_red:
      m_light_diffuse[0] = 1.f;
      break;
  case EColor::e_white:
      m_light_diffuse[0] = 1.f;
      m_light_diffuse[1] = 1.f;
      m_light_diffuse[2] = 1.f;
      break;
  }
  glLightfv(get_id(), GL_DIFFUSE, m_light_diffuse);
}

// ustawienie katow na wartosci z przedzialu  -pi - pi
void PointLight::normalize_angles() {
  if (m_az_angle > M_PI)
    m_az_angle -= 2 * M_PI;
  if (m_az_angle <= -M_PI)
    m_az_angle += 2 * M_PI;

  if (m_el_angle > M_PI)
    m_el_angle -= 2 * M_PI;
  if (m_el_angle <= -M_PI)
    m_el_angle += 2 * M_PI;
}
