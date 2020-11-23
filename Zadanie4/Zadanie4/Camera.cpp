// Copyright 2020 Bartosz Panus

#define _USE_MATH_DEFINES
#include "Camera.h"
#include <math.h>
#include <iostream>

Camera::Camera() :
  m_az_angle{ 0.f },
  m_el_angle{ 0.f },
  m_distance{ 20.f },
  m_reset_az_ang{ m_az_angle },
  m_reset_el_ang{ m_el_angle },
  m_reset_distance{ m_distance } {
    m_look_at[0] = 0.;
    m_look_at[1] = 0.;
    m_look_at[2] = 0.;

    update_pos();
}

Camera::Camera(float az_ang, float el_ang,
  float distance, GLfloat point_to_look[3]):
  m_az_angle{ az_ang },
  m_el_angle{ el_ang },
  m_distance{ distance },
  m_reset_az_ang{ m_az_angle },
  m_reset_el_ang{ m_el_angle },
  m_reset_distance{ m_distance } {
    m_look_at[0] = point_to_look[0];
    m_look_at[1] = point_to_look[1];
    m_look_at[2] = point_to_look[2];
    normalize_angles();
    update_pos();
}

// ustawienie katow azymutu i elewacji
void Camera::set_angles(float az_ang, float el_ang) {
  m_az_angle = az_ang;
  m_el_angle = el_ang;
  normalize_angles();

  update_pos();
}

// dodanie wartosci katow azymutu i elewacji
void Camera::add_angles(float az_ang, float el_ang) {
  m_az_angle += az_ang;
  m_el_angle += el_ang;

  normalize_angles();
  update_pos();
}

// ustawienie dystansu kamery od punktu (0,0,0)
void Camera::set_distance(float dist) {
  m_distance = dist;
  update_pos();
}

// zwiekszenie dystansu o dana wartosc
void Camera::add_distance(float dist) {
  m_distance += dist;
  update_pos();
}

// reset ustawien kamery do wartosci poczatkowych
void Camera::reset() {
  m_az_angle = m_reset_az_ang;
  m_el_angle = m_reset_el_ang;
  m_distance = m_reset_distance;
  normalize_angles();
  update_pos();
}

// obliczanie pozycji kamery na bazie katow i dystansu
void Camera::update_pos() {
  m_pos[0] = m_distance * cos(m_az_angle) * cos(m_el_angle);
  m_pos[1] = m_distance * sin(m_el_angle);
  m_pos[2] = m_distance * sin(m_az_angle) * cos(m_el_angle);
}

// ustawienie macierzy lookat
void Camera::set_matrix() {
  if ((m_el_angle > M_PI_2) || (m_el_angle <= -(M_PI_2))) {
    vec_up = -1.f;
  } else {
      vec_up = 1.f;
  }
  gluLookAt(m_pos[0], m_pos[1], m_pos[2], m_look_at[0],
    m_look_at[1], m_look_at[2], 0.0, vec_up, 0.0);
}

// ustawienie katow na wartosci z przedzialu  -pi - pi
void Camera::normalize_angles() {
  if (m_az_angle > M_PI)
    m_az_angle -= 2 * M_PI;
  if (m_az_angle <= -M_PI)
    m_az_angle += 2 * M_PI;

  if (m_el_angle > M_PI)
    m_el_angle -= 2 * M_PI;
  if (m_el_angle <= -M_PI)
    m_el_angle += 2 * M_PI;
}
