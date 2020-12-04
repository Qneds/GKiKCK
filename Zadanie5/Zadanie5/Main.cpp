// Copyright 2020 Bartosz Panus

/*******************************************/
// Najprostszy program wykorzystujacy funkcje biblioteki OpenGL
/******************************************/
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <time.h>
#include <iostream>
#include <thread> // NOLINT
#include <chrono> // NOLINT
#include "Camera.h"
#include "DirectionalLight.h"
#include "Egg.h"
#include "Mouse.h"
#include "PointLight.h"
#include "Texture.h"
/*****************************************/
static GLfloat pix2angle;  // przelicznik pikseli na stopnie
float g_ang_speed = 0.1f;
constexpr float G_DISTANCE_PER_SCROLL = 1.;

typedef float poin3d[3];

poin3d cords{0., 0., 0.};

// do odroznienia obrotow
bool rot = false;

// wlaczanie i wylaczanie swiatla
bool is_light_on = true;

static GLfloat viewer[] = {0.0, 0.0, 20.0};

Egg* egg;
MyMouse g_mouse;
Camera g_camera;
PointLight* g_point_light;
DirectionalLight* g_dir_light;

Texture t1, t2, t3, t4;

// Funkcaja okreslajaca, co ma byc rysowane
// (zawsze wywolywana, gdy trzeba przerysowac scene)
void RenderScene(void) {
  glMatrixMode(GL_MODELVIEW);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Czyszczenie okna aktualnym kolorem czyszczacym

  egg->material();
  // ustawienie materialu

  egg->texture();

  glLoadIdentity();
  // Czyszczenie macierzy biezacej
  if (g_mouse.get_state() & MyMouse::e_right_button) {
    g_camera.add_angles(g_ang_speed * pix2angle * g_mouse.get_delta_x(),
                        g_ang_speed * pix2angle * g_mouse.get_delta_y());
  }
  if (g_mouse.get_state() & MyMouse::e_left_button) {
    g_point_light->add_angles(
        -1 * g_ang_speed * pix2angle * g_mouse.get_delta_x(),
        -1 * g_ang_speed * pix2angle * g_mouse.get_delta_y());
  }

  glPushMatrix();
  g_camera.set_matrix();

  g_point_light->set_light_in_scene();
  g_dir_light->set_light_in_scene();

  egg->draw();

  glPopMatrix();
  glFlush();
  // Przekazanie polecen rysujacych do wykonania

  glutSwapBuffers();
}
/*****************************************/

// funkcja wywolujlca obroty jajek
void spin() {
  egg->spin_egg();
  g_dir_light->spin();
  glutPostRedisplay();  // odswiezenie zawartosci aktualnego okna
}

// Funkcja "bada" stan myszy i ustawia
// wartosci odpowiednich zmiennych globalnych
void Mouse(int btn, int state, int x, int y) {
  g_mouse.update(btn, state, x, y);
}
/*****************************************/

// Funkcja "monitoruje" polozenie kursora myszy i ustawia wartosci odpowiednich
// zmiennych globalnych
void Motion(GLsizei x, GLsizei y) {
  g_mouse.update_pos(x, y);
  glutPostRedisplay();  // przerysowanie obrazu sceny
}
/*****************************************/

// funkcja do oblugi klawiszy
void keys(unsigned char key, int x, int y) {
  if (key == 'z') {
    if (rot) {
      egg->set_spin_theta(false, true, false);
      rot = false;
    } else {
      egg->set_spin_theta(true, false, false);
      rot = true;
    }
  }
  if (key == '+') {
    g_camera.add_distance(-G_DISTANCE_PER_SCROLL);
  }
  if (key == '-') {
    g_camera.add_distance(G_DISTANCE_PER_SCROLL);
  }
  if (key == '/') {
    g_camera.reset();
  }
  if (key == 'r') {
    g_point_light->set_color(PointLight::e_red);
  }
  if (key == 'g') {
    g_point_light->set_color(PointLight::e_green);
  }
  if (key == 'b') {
    g_point_light->set_color(PointLight::e_blue);
  }
  if (key == 'w') {
    g_point_light->set_color(PointLight::e_white);
  }
  if (key == 'l') {
    if (is_light_on) {
      glDisable(GL_LIGHTING);
      is_light_on = false;
    } else {
      glEnable(GL_LIGHTING);
      is_light_on = true;
    }
  }

  RenderScene();  // przerysowanie obrazu sceny
}


// funkcja do uruchomienia w watku. Zmienia obecna teksutre co 3 sekundy
void switching_textures() {
  int num_of_miliseconds{3000};
  while (true) {
    egg->set_texture(t1);
    std::this_thread::sleep_for(std::chrono::milliseconds(num_of_miliseconds));
    egg->set_texture(t2);
    std::this_thread::sleep_for(std::chrono::milliseconds(num_of_miliseconds));
    egg->set_texture(t3);
    std::this_thread::sleep_for(std::chrono::milliseconds(num_of_miliseconds));
    egg->set_texture(t4);
    std::this_thread::sleep_for(std::chrono::milliseconds(num_of_miliseconds));
  }
}


// Funkcja ustalajaca stan renderowania
void MyInit(void) {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  // Kolor czyszczacy (wypelnienia okna) ustawiono na czarny


  /*************************************************/
  // Zmienne dla obrazu tekstury
  GLbyte* pBytes;
  GLint ImWidth, ImHeight, ImComponents;
  GLenum ImFormat;
  uint16_t sdepth;


  //  Przeczytanie obrazu tekstury z pliku o nazwie textur2.tga
  pBytes = LoadTGAImage("textur2.tga", &ImWidth, &ImHeight,
    &ImComponents, &ImFormat, &sdepth);

  int len = ImWidth * ImHeight * sdepth/4;

  GLbyte* bytes1 = new GLbyte[len];
  GLbyte* bytes2 = new GLbyte[len];
  GLbyte* bytes3 = new GLbyte[len];
  GLbyte* bytes4 = new GLbyte[len];


  int half_w{ImWidth / 2}, half_h{ImHeight / 2};


  // podzielenie jednej tektury na 4 mniejsze
  for (size_t i{0}; i < ImWidth; i++) {
    for (size_t j{0}; j < ImHeight; j++) {
      if (i < half_w && j < half_h) {
        for (size_t k{0}; k < sdepth; k++) {                      // *|
          bytes1[i * half_w * sdepth + j * sdepth + k] =      // ---
              pBytes[i * ImWidth * sdepth + j * sdepth + k];   //  |
        }
      } else if (i < half_w && j >= half_h) {
        for (size_t k{0}; k < sdepth; k++) {                      //  |*
          bytes2[i * half_w * sdepth + (j - half_h) * sdepth +
                 k] =                                             // ---
              pBytes[i * ImWidth * sdepth + j * sdepth + k];   //  |
        }
      } else if (i >= half_w && j < half_h) {
        for (size_t k{0}; k < sdepth; k++) {                      //  |
          bytes3[(i - half_w) * half_w * sdepth + j * sdepth + k] =  // ---
              pBytes[i * ImWidth * sdepth + j * sdepth + k];         // *|
        }
      } else {
        for (size_t k{0}; k < sdepth; k++) {                      //  |
          bytes4[(i - half_w) * half_w * sdepth + (j - half_h) * sdepth +
                 k] =                                         // ---
              pBytes[i * ImWidth * sdepth + j * sdepth + k];  //  |*
        }
      }
    }
  }


  t1 = Texture{bytes1, ImWidth / 2, ImHeight / 2,
    ImComponents, ImFormat, sdepth};
  t2 = Texture{bytes2, ImWidth / 2, ImHeight / 2,
    ImComponents, ImFormat, sdepth};
  t3 = Texture{bytes3, ImWidth / 2, ImHeight / 2,
    ImComponents, ImFormat, sdepth};
  t4 = Texture{bytes4, ImWidth / 2, ImHeight / 2,
    ImComponents, ImFormat, sdepth};


  new std::thread{switching_textures};

  delete[] bytes1;
  delete[] bytes2;
  delete[] bytes3;
  delete[] bytes4;



  /*********************************************************/
  // Zwolnienie pamieci
  delete[] pBytes;
  /*********************************************************/

  // Wlaczenie mechanizmu teksturowania
  glEnable(GL_TEXTURE_2D);

  /*********************************************************/
  // Ustalenie trybu teksturowania
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  /*********************************************************/
  // Okreslenie sposobu nakladania tekstur
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



  GLfloat light_ambient[] = {0.1, 0.1, 0.1, 1.0};
  // skladowe intensywnosci swiecenia zrodla swiatla otoczenia
  // Ia = [Iar,Iag,Iab]

  GLfloat light_diffuse[] = {1., 0., 0, 1.0};
  // skladowe intensywnosci swiecenia zrodla swiatla powodujacego
  // odbicie dyfuzyjne Id = [Idr,Idg,Idb]

  GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
  // skladowe intensywnosci swiecenia zrodla swiatla powodujacego
  // odbicie kierunkowe Is = [Isr,Isg,Isb]

  GLfloat att_constant = {1.0};
  // skladowa stala ds dla modelu zmian oswietlenia w funkcji
  // odleglosci od zrodla

  GLfloat att_linear = {0.05};
  // skladowa liniowa dl dla modelu zmian oswietlenia w funkcji
  // odleglosci od zrodla

  GLfloat att_quadratic = {0.001};
  // skladowa kwadratowa dq dla modelu zmian oswietlenia w funkcji
  // odleglosci od zrodla

  g_point_light =
      new PointLight(light_ambient, PointLight::e_green, light_specular,
                     att_constant, att_linear, att_quadratic);

  g_dir_light =
      new DirectionalLight(light_ambient, light_diffuse, light_specular,
                           att_constant, att_linear, att_quadratic);



  /*************************************************************/
  // Ustawienie opcji systemu oswietlania sceny

  glShadeModel(GL_SMOOTH);  // wlaczenie lagodnego cieniowania
  glEnable(GL_LIGHTING);    // wlaczenie systemu oswietlenia sceny

  glEnable(GL_DEPTH_TEST);  // wlaczenie mechanizmu z-bufora

  /*************************************************************/
}
/*****************************************/

void ChangeSize(GLsizei horizontal, GLsizei vertical) {
  // przeliczenie pikseli na stopnie
  pix2angle = 360.0 / static_cast<float>(horizontal);
  glMatrixMode(GL_PROJECTION);
  // Przelaczenie macierzy biezacej na macierz projekcji

  glLoadIdentity();
  // Czyszcznie macierzy biezacej

  gluPerspective(70, 1.0, 1.0, 100.0);
  // Ustawienie parametrow dla rzutu perspektywicznego

  if (horizontal <= vertical)
    glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);
  else
    glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
  // Ustawienie wielkosci okna okna widoku (viewport) w zaleznosci
  // relacji pomiedzy wysokoscia i szerokoscia okna

  glMatrixMode(GL_MODELVIEW);
  // Przelaczenie macierzy biezacej na macierz widoku modelu

  glLoadIdentity();
  // Czyszczenie macierzy biezacej
}

/*****************************************/
// Glowny punkt wejscia programu. Program dziala w trybie konsoli
int main(void) {
  // tworzenie jajka
  unsigned int N;
  std::cout << "Podaj N: ";
  std::cin >> N;
  std::cout << "Z - zmiana obrotow" << std::endl;
  std::cout << "-/+ - zoom" << std::endl;
  std::cout << "/ - reset kamery" << std::endl;
  std::cout << "R - kolor czerwony zrodla punktowego" << std::endl;
  std::cout << "G - kolor zielony zrodla punktowego" << std::endl;
  std::cout << "B - kolor niebieski zrodla punktowego" << std::endl;
  std::cout << "W - kolor bialy zrodla punktowego" << std::endl;
  std::cout << "L - wlaczanie/wylacznie swiatla" << std::endl;
  std::cout << "LPM - poruszanie swiatlem punktowym" << std::endl;
  std::cout << "PPM - poruszanie kamera" << std::endl;

  egg = new Egg{N, cords, 1.};
  egg->set_spin_theta(false, false, false);

  // tworzenie jajka-satelity

  srand(time(NULL));
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  glutInitWindowSize(300, 300);

  glutCreateWindow("Uklad wspolrzednych 3-D");

  glutDisplayFunc(RenderScene);
  // Okreslenie, ze funkcja RenderScene bedzie funkcja zwrotna
  // (callback function).  Bedzie ona wywolywana za kazdym razem
  // gdy zajdzie potrzba przeryswania okna

  glutIdleFunc(spin);

  glutMouseFunc(Mouse);
  glutMotionFunc(Motion);
  // glutPassiveMotionFunc(Motion);
  glutKeyboardFunc(keys);

  glutReshapeFunc(ChangeSize);
  // Dla aktualnego okna ustala funkcje zwrotna odpowiedzialna
  // zazmiany rozmiaru okna

  MyInit();
  // Funkcja MyInit() (zdefiniowana powyzej) wykonuje wszelkie
  // inicjalizacje konieczne  przed przystapieniem do renderowania

  glEnable(GL_DEPTH_TEST);
  // Wlaczenie mechanizmu usuwania powierzchni niewidocznych

  glutMainLoop();
  // Funkcja uruchamia szkielet biblioteki GLUT
}
/*****************************************/
