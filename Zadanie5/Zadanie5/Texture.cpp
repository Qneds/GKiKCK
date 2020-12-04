// Copyright 2020 Bartosz Panus

#include "Texture.h"
#include <fstream>

Texture::Texture(std::string file_name) {
  GLbyte* bytes =
      LoadTGAImage(file_name, &m_width, &m_height, &m_components,
        &m_format, &m_sdepth);
  int tmp = m_width * m_height * m_sdepth;

  if (bytes == nullptr) {
    m_ptr_bytes = nullptr;
    return;
  }

  m_ptr_bytes = new GLbyte[tmp];

  for (int i{0}; i < tmp; i++) {
    m_ptr_bytes[i] = bytes[i];
  }
  delete[] bytes;
}

Texture::Texture(GLbyte* bytes, GLint width, GLint height, GLint components,
                 GLenum format, uint16_t sdepth) {
  m_width = width;
  m_height = height;
  m_components = components;
  m_format = format;
  m_sdepth = sdepth;
  int tmp = m_width * m_height * m_sdepth;

  if (bytes == nullptr) {
    m_ptr_bytes = nullptr;
    return;
  }

  m_ptr_bytes = new GLbyte[tmp];

  for (int i{0}; i < tmp; i++) {
    m_ptr_bytes[i] = bytes[i];
  }
}

Texture::Texture(const Texture& text) {
  int tmp = text.m_width * text.m_height * text.m_sdepth;
  m_width = text.m_width;
  m_height = text.m_height;
  m_components = text.m_components;
  m_format = text.m_format;
  m_sdepth = text.m_sdepth;

  if (text.m_ptr_bytes == nullptr) {
    m_ptr_bytes = nullptr;
    return;
  }

  m_ptr_bytes = new GLbyte[tmp];

  for (int i{0}; i < tmp; i++) {
    m_ptr_bytes[i] = text.m_ptr_bytes[i];
  }
}

Texture& Texture::operator=(const Texture& text) {
  if (this != &text) {
    delete[] m_ptr_bytes;

    int tmp = text.m_width * text.m_height * text.m_sdepth;
    m_width = text.m_width;
    m_height = text.m_height;
    m_components = text.m_components;
    m_format = text.m_format;
    m_sdepth = text.m_sdepth;

    if (text.m_ptr_bytes == nullptr) {
      m_ptr_bytes = nullptr;
    } else {
      m_ptr_bytes = new GLbyte[tmp];
      for (int i{0}; i < tmp; i++) {
        m_ptr_bytes[i] = text.m_ptr_bytes[i];
      }
    }
  }
  return *this;
}

Texture::~Texture() {
  delete[] m_ptr_bytes; }


// ustawienie tekstury jako obecnie wybranej
void Texture::set_up_texture() {
  if (m_ptr_bytes == nullptr) return;

  glTexImage2D(GL_TEXTURE_2D, 0, m_components, m_width, m_height, 0, m_format,
               GL_UNSIGNED_BYTE, m_ptr_bytes);
}

// wczytywanie tekstury
GLbyte* LoadTGAImage(std::string file_name, GLint* ptr_width, GLint* ptr_height,
                     GLint* ptr_components, GLenum* ptr_format,
                     uint16_t* ptr_depth) {
  /************************************************************************/
  // Struktura dla nagloka pliku  TGA
#pragma pack(1)
  typedef struct {
    GLbyte idlength;
    GLbyte colormaptype;
    GLbyte datatypecode;
    uint16_t colormapstart;
    uint16_t colormaplength;
    unsigned char colormapdepth;
    uint16_t x_orgin;
    uint16_t y_orgin;
    uint16_t width;
    uint16_t height;
    GLbyte bitsperpixel;
    GLbyte descriptor;
  } TGAHEADER;
#pragma pack(8)

  std::ifstream in;
  TGAHEADER tgaHeader;
  uint64_t lImageSize;
  int16_t sDepth;
  GLbyte* pbitsperpixel = nullptr;

/************************************************************************/
  // Wartosci domyslne zwracane w przypadku bledu
  *ptr_width = 0;
  *ptr_height = 0;
  *ptr_format = GL_BGR_EXT;
  *ptr_components = GL_RGB8;
  *ptr_depth = 0;

  in.open(file_name, std::ios::binary);
  if (!in.is_open()) return nullptr;

/************************************************************************/
  // Przeczytanie naglowka pliku
  size_t t = sizeof(TGAHEADER);
  in.read(reinterpret_cast<char*>(&tgaHeader), t);


/************************************************************************/
  // Odczytanie szerokosci, wysokosci i glebi obrazu
  *ptr_width = tgaHeader.width;
  *ptr_height = tgaHeader.height;
  sDepth = tgaHeader.bitsperpixel / 8;

/************************************************************************/
  // Sprawdzenie, czy glebia spelnia zalozone warunki (8, 24, lub 32 bity)
  if (tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24 &&
      tgaHeader.bitsperpixel != 32)
    return nullptr;

/************************************************************************/
  // Obliczenie rozmiaru bufora w pamieci
  lImageSize = tgaHeader.width * tgaHeader.height * sDepth;

/************************************************************************/
  // Alokacja pamieci dla danych obrazu
  pbitsperpixel = new GLbyte[lImageSize];

  if (pbitsperpixel == nullptr) return nullptr;

  in.read(reinterpret_cast<char*>(pbitsperpixel), lImageSize);

  if (in.fail()) {
    delete[] pbitsperpixel;
    return nullptr;
  }

  *ptr_depth = sDepth;
/************************************************************************/
  // Ustawienie formatu OpenGL
  switch (sDepth) {
    case 3:
      *ptr_format = GL_BGR_EXT;
      *ptr_components = GL_RGB8;
      break;
    case 4:
      *ptr_format = GL_BGRA_EXT;
      *ptr_components = GL_RGBA8;
      break;
    case 1:
      *ptr_format = GL_LUMINANCE;
      *ptr_components = GL_LUMINANCE8;
      break;
  }

  return pbitsperpixel;
}
