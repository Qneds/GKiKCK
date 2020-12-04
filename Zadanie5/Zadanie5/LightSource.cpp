// Copyright 2020 Bartosz Panus

#include "LightSource.h"

bool LightSource::m_ba_is_index_used[10]{ false };
size_t LightSource::m_used_indexes = 0;

LightSource::LightSource() {
  if (m_used_indexes > 10) {
    m_b_is_created = false;
    return;
  } else {
      create();
  }
}

LightSource::~LightSource() {
  size_t id{ m_light_id - GL_LIGHT0 };
  m_ba_is_index_used[id] = false;
  m_used_indexes--;
  glDisable(m_light_id);
}

bool LightSource::try_create() {
  if (m_used_indexes > 10) {
    m_b_is_created = false;
    return m_b_is_created;
  } else if (m_b_is_created == true) {
    return m_b_is_created;
  }
  create();
  return m_b_is_created;
}


void LightSource::create() {
  for (int i{ 0 }; i < 10; i++) {
    if (m_ba_is_index_used[i] == false) {
      m_ba_is_index_used[i] = true;
      m_b_is_created = true;
      m_b_light_enabled = true;
      m_used_indexes++;
      m_light_id = GL_LIGHT0 + i;
      glEnable(m_light_id);
      break;
    }
  }
}
