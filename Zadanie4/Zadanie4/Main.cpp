// Copyright 2020 Bartosz Panus

/*******************************************/
// Najprostszy program wykorzystujacy funkcje biblioteki OpenGL
/******************************************/
#include <windows.h>
#include <time.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include "Egg.h"
#include <iostream>
#include "Mouse.h"
#include "Camera.h"
#include "PointLight.h"
#include "DirectionalLight.h"
/*****************************************/
static GLfloat pix2angle;     // przelicznik pikseli na stopnie
float g_ang_speed = 0.1f;
constexpr float G_DISTANCE_PER_SCROLL = 1.;

typedef float poin3d[3];

poin3d cords{ 0., 0., 0. };

// do odroznienia obrotow
bool rot = false;

static GLfloat viewer[] = { 0.0, 0.0, 20.0 };

Egg* egg;
MyMouse g_mouse;
Camera g_camera;
PointLight* g_point_light;
DirectionalLight* g_dir_light;


// Funkcaja okreslajaca, co ma byc rysowane
// (zawsze wywolywana, gdy trzeba przerysowac scene)
void RenderScene(void) {
  glMatrixMode(GL_MODELVIEW);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// Czyszczenie okna aktualnym kolorem czyszczacym

  glLoadIdentity();
// Czyszczenie macierzy biezacej
  if (g_mouse.get_state() & MyMouse::e_right_button) {
    g_camera.add_angles(g_ang_speed * pix2angle * g_mouse.get_delta_x(),
      g_ang_speed * pix2angle * g_mouse.get_delta_y());
  }
  if (g_mouse.get_state() & MyMouse::e_left_button) {
    g_point_light->add_angles(-1 * g_ang_speed * pix2angle *
      g_mouse.get_delta_x(), -1 * g_ang_speed * pix2angle *
      g_mouse.get_delta_y());
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

  RenderScene();  // przerysowanie obrazu sceny
}


// Funkcja ustalajaca stan renderowania
void MyInit(void) {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
// Kolor czyszczacy (wypelnienia okna) ustawiono na czarny

  /*************************************************************/

//  Definicja materialu z jakiego zrobione jest jajko
//  i definicja zrodla swiatla

  /*************************************************************/


  /*************************************************************/
// Definicja materialu z jakiego zrobione jest jajko

  GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
  // wspolczynniki ka =[kar,kag,kab] dla swiatla otoczenia

  GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
  // wspolczynniki kd =[kdr,kdg,kdb] swiatla rozproszonego

  GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  // wspolczynniki ks =[ksr,ksg,ksb] dla swiatla odbitego

  GLfloat mat_shininess = { 20.0 };
  // wspolczynnik n opisujacy polysk powierzchni


  /*************************************************************/
// Definicja zrodla swiatla


  // polozenie zrodla swiatla


  GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
  // skladowe intensywnosci swiecenia zrodla swiatla otoczenia
  // Ia = [Iar,Iag,Iab]

  GLfloat light_diffuse[] = { 1., 0., 0, 1.0 };
  // skladowe intensywnosci swiecenia zrodla swiatla powodujacego
  // odbicie dyfuzyjne Id = [Idr,Idg,Idb]

  GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  // skladowe intensywnosci swiecenia zrodla swiatla powodujacego
  // odbicie kierunkowe Is = [Isr,Isg,Isb]

  GLfloat att_constant = { 1.0 };
  // skladowa stala ds dla modelu zmian oswietlenia w funkcji
  // odleglosci od zrodla

  GLfloat att_linear = { 0.05 };
  // skladowa liniowa dl dla modelu zmian oswietlenia w funkcji
  // odleglosci od zrodla

  GLfloat att_quadratic = { 0.001 };
  // skladowa kwadratowa dq dla modelu zmian oswietlenia w funkcji
  // odleglosci od zrodla


  g_point_light = new PointLight(light_ambient, PointLight::e_green,
    light_specular, att_constant, att_linear, att_quadratic);

  g_dir_light = new DirectionalLight(light_ambient, light_diffuse,
    light_specular, att_constant, att_linear, att_quadratic);


  /*************************************************************/
// Ustawienie parametrow materialu i zrodla swiatla

  /*************************************************************/
// Ustawienie parametrow materialu


  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);



  /*************************************************************/
  // Ustawienie opcji systemu oswietlania sceny

  glShadeModel(GL_SMOOTH);  // wlaczenie lagodnego cieniowania
  glEnable(GL_LIGHTING);   // wlaczenie systemu oswietlenia sceny

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
void main(void) {
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
