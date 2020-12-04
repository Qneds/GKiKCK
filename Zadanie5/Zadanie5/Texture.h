// Copyright 2020 Bartosz Panus

#pragma once
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <string>

GLbyte* LoadTGAImage(std::string file_name, GLint* ptr_width, GLint* ptr_height,
                     GLint* ptr_components, GLenum* ptr_format,
                     uint16_t* ptr_depth);

class Texture {
 public:
  Texture() = default;
  explicit Texture(std::string file_name);
  Texture(GLbyte* bytes, GLint width, GLint height, GLint components,
          GLenum format, uint16_t sdepth);
  Texture(const Texture& text);
  ~Texture();

  Texture& operator=(const Texture& text);
  void set_up_texture();

 private:
  GLbyte *m_ptr_bytes;
  GLint m_width, m_height, m_components;
  GLenum m_format;
  uint16_t m_sdepth;
};
