// Copyright 2020 Bartosz Panus

#include "Egg.h"
#define _USE_MATH_DEFINES
#include <math.h>

Egg::Egg(unsigned N, const point3d cords, float scale) :
  m_scale_val{ scale },
  m_N{ N },
  m_dist{ 1.f / m_N } {
    m_points = new point3d * [m_N];
    m_normals = new point3d * [m_N];

    for (int i{ 0 }; i < m_N; i++) {
      m_points[i] = new point3d[m_N];
      m_normals[i] = new point3d[m_N];

      if (i < 3) {
        m_cords[i] = cords[i];
      }
    }
    generate_points();
}

Egg::~Egg() {
  for (int i{ 0 }; i < m_N; i++) {
    delete[] m_points[i];
    delete[] m_normals[i];
  }

  delete m_normals;
  delete[] m_points;
}

void Egg::draw() {
// ustawianie orientacji i pozycji
  glTranslatef(0., 5., 0.);

  glRotatef(m_spin[0], 1.0, 0.0, 0.0);
  glRotatef(m_spin[1], 0.0, 1.0, 0.0);
  glRotatef(m_spin[2], 0.0, 0.0, 1.0);

  glTranslatef(0., -5., 0.);

  glTranslatef(m_cords[0], m_cords[1], m_cords[2]);

  glScalef(m_scale_val, m_scale_val, m_scale_val);

  glColor3f(1.0f, 1.0f, 1.0f);


  glColor3f(0.25f, 0.25f, 0.25f);

  for (int i = 0; i < m_N; i++) {
    for (int j = 0; j < m_N; j++) {
      if ((j + 1) != m_N) {
        glBegin(GL_TRIANGLES);
        glNormal3fv(m_normals[i][j]);
        glVertex3fv(m_points[i][j]);
        glNormal3fv(m_normals[(i + 1) % m_N][j]);
        glVertex3fv(m_points[(i + 1) % m_N][j]);
        glNormal3fv(m_normals[i][j + 1]);
        glVertex3fv(m_points[i][j + 1]);
        glEnd();

        glBegin(GL_TRIANGLES);
        glNormal3fv(m_normals[(i + 1) % m_N][j]);
        glVertex3fv(m_points[(i + 1) % m_N][j]);
        glNormal3fv(m_normals[(i + 1) % m_N][j + 1]);
        glVertex3fv(m_points[(i + 1) % m_N][j + 1]);
        glNormal3fv(m_normals[i][j + 1]);
        glVertex3fv(m_points[i][j + 1]);
        glEnd();
      } else {
        if (i > 0) {
          glBegin(GL_TRIANGLES);
          glNormal3fv(m_normals[i][j]);
          glVertex3fv(m_points[i][j]);
          glNormal3fv(m_normals[(i + 1) % m_N][j]);
          glVertex3fv(m_points[(i + 1) % m_N][j]);
          glNormal3fv(m_normals[m_N - i][0]);
          glVertex3fv(m_points[m_N - i][0]);
          glEnd();
        }
        glBegin(GL_TRIANGLES);
        glNormal3fv(m_normals[(i + 1) % m_N][j]);
        glVertex3fv(m_points[(i + 1) % m_N][j]);
        glNormal3fv(m_normals[(m_N - i) % m_N][0]);
        glVertex3fv(m_points[(m_N - i) % m_N][0]);
        glNormal3fv(m_normals[m_N - i - 1][0]);
        glVertex3fv(m_points[m_N - i - 1][0]);
        glEnd();
      }
    }
  }
}



// wzory do liczenia pocgodnych
float Egg::fxu(float u, float v) {
  return (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 *
    pow(u, 2) + 360 * u - 45) * cos(M_PI * v);
}

float Egg::fxv(float u, float v) {
  return M_PI * (90 * pow(u, 5) - 225 * pow(u, 4) +
    270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u) * sin(M_PI * v);
}

float Egg::fyu(float u, float v) {
  return 640 * pow(u, 3) - 960 * pow(u, 2) + 320 * u;
}

float Egg::fyv(float u, float v) {
  return 0.0f;
}

float Egg::fzu(float u, float v) {
  return (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 *
    pow(u, 2) + 360 * u - 45) * sin(M_PI * v);
}

float Egg::fzv(float u, float v) {
  return -M_PI * (90 * pow(u, 5) - 225 * pow(u, 4) +
    270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u) * cos(M_PI * v);
}

float Egg::length(point3d** arr, int i, int j) {
  return sqrt((arr[i][j][0] * arr[i][j][0] + arr[i][j][1] *
    arr[i][j][1] + arr[i][j][2] * arr[i][j][2]));
}

// generowanie chmury punktow
void Egg::generate_points() {
  float u_off{ 0.}, v_off;

  for (int u{ 0 }; u < m_N; u++) {
    v_off = 0.;

    for (int v{ 0 }; v < m_N; v++) {
      float p = ((-90) * powf(u_off, 5) + 225 *
        powf(u_off, 4) - 270 * powf(u_off, 3) +
        180 * powf(u_off, 2) - 45 * u_off);

      m_points[u][v][0] = p * cos(M_PI * v_off);
      m_points[u][v][1] = 160 * powf(u_off, 4) - 320 *
        powf(u_off, 3) + 160 * powf(u_off, 2);
      m_points[u][v][2] = p * sin(M_PI * v_off);


      float xv{ fxv(u_off, v_off) };
      float xu{ fxu(u_off, v_off) };
      float yv{ fyv(u_off, v_off) };
      float yu{ fyu(u_off, v_off) };
      float zv{ fzv(u_off, v_off) };
      float zu{ fzu(u_off, v_off) };

      // liczenie normalnych
      m_normals[u][v][0] = yu * zv - zu * yv;
      m_normals[u][v][1] = zu * xv - xu * zv;
      m_normals[u][v][2] = xu * yv - yu * xv;

      float len = length(m_normals, u, v);

      if (len == 0) {
          if (u == 0) m_normals[u][v][1] = -1;
          len = 1;
      }


      if (u >= (m_N / 2)) {
         m_normals[u][v][0] *= -1;
         m_normals[u][v][1] *= -1;
         m_normals[u][v][2] *= -1;
      }

      // normalizacja
      m_normals[u][v][0] /= len;
      m_normals[u][v][1] /= len;
      m_normals[u][v][2] /= len;


      v_off += m_dist;
    }
    u_off += m_dist;
  }
}


float Egg::rand_color() {
  return rand() % 1000 / 1000.;
}

// okreslenie konta obrotu wokol danej osi
void Egg::spin_egg() {
  if (m_spinning[0]) {
    m_spin[0] -= 0.5;
    if (m_spin[0] > 360.0) m_spin[0] -= 360.0;
  }

  if (m_spinning[1]) {
    m_spin[1] -= 0.5;
    if (m_spin[1] > 360.0) m_spin[1] -= 360.0;
  }

  if (m_spinning[2]) {
    m_spin[2] -= 0.5;
    if (m_spin[2] > 360.0) m_spin[2] -= 360.0;
  }
}


void Egg::set_spin_theta(bool x, bool y, bool z) {
// ustawienie wokol ktorych osi ma nastepowac obrot
  m_spinning[0] = x;
  m_spinning[1] = y;
  m_spinning[2] = z;

  m_spin[0] = 0.;
  m_spin[1] = 0.;
  m_spin[2] = 0.;
}


