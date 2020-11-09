/*************************************************************************************/
// Najprostszy program wykorzystuj�cy funkcje biblioteki OpenGL
/*************************************************************************************/
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include "Egg.h"
#include <iostream>
#include <time.h>
#include "Mouse.h"
#include "Camera.h"
/*************************************************************************************/
static GLfloat pix2angle;     // przelicznik pikseli na stopnie
float g_ang_speed = 0.1f;
constexpr float G_DISTANCE_PER_SCROLL = 1.;

typedef float poin3d[3];

poin3d cords{ 0., 0., 0. };

//do odr�znienia obrot�w
bool rot = false;

static GLfloat viewer[] = { 0.0, 0.0, 20.0 };

Egg* egg, *sat_egg;
MyMouse g_mouse;
Camera g_camera;




// Funkcaja okre�laj�ca, co ma by� rysowane
// (zawsze wywo�ywana, gdy trzeba przerysowa� scen�)
void RenderScene(void)
{

    
    glMatrixMode(GL_MODELVIEW); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Czyszczenie okna aktualnym kolorem czyszcz�cym

    glLoadIdentity();
    // Czyszczenie macierzy bie��cej
    if (g_mouse.get_state() & MyMouse::e_left_button)
    {
        g_camera.add_angles(g_ang_speed * pix2angle * g_mouse.get_delta_x(), g_ang_speed * pix2angle * g_mouse.get_delta_y());
    }

    g_camera.set_matrix();
    
                               

    //glRotatef(theta, 0.0, 1.0, 0.0);  //obr�t obiektu o nowy k�t


    //glTranslated(0, -4, 0);
    //glRotated(-35.0, 1., 0., 0.);

    egg->draw();
    //glLoadIdentity();

    //glTranslated(0, -4, 0);
    //glRotated(-35.0, 1., 0., 0.);
    sat_egg->draw();
    
   

    glFlush();
    // Przekazanie polece� rysuj�cych do wykonania


    glutSwapBuffers();
    //
}
/*************************************************************************************/


//funkcja wywo�uj�ca obroty jajek
void spin() {

    egg->spin_egg();
    sat_egg->spin_egg();
}



// Funkcja "bada" stan myszy i ustawia warto�ci odpowiednich zmiennych globalnych
void Mouse(int btn, int state, int x, int y)
{

    g_mouse.update(btn, state, x, y);

}
/*************************************************************************************/

// Funkcja "monitoruje" po�o�enie kursora myszy i ustawia warto�ci odpowiednich
// zmiennych globalnych
void Motion(GLsizei x, GLsizei y)
{

    g_mouse.update_pos(x, y);

    glutPostRedisplay();     // przerysowanie obrazu sceny
}
/*************************************************************************************/


//funkcja do ob�ugi klawiszy
void keys(unsigned char key, int x, int y) {

    if (key == 'p') {
        egg->set_state(Egg::e_points);
        sat_egg->set_state(Egg::e_points);
    }
    if (key == 'l') {
        egg->set_state(Egg::e_lines);
        sat_egg->set_state(Egg::e_lines);
    }
    if (key == 't') {
        egg->set_state(Egg::e_triangles);
        sat_egg->set_state(Egg::e_triangles);
    }
    if (key == 'z') {

        if (rot) {
            egg->set_spin_theta(false, true, false);
            rot = false;
        }
        else {
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
    if (key == 'r') {
        g_camera.reset();
    }

    RenderScene(); // przerysowanie obrazu sceny
}


// Funkcja ustalaj�ca stan renderowania
void MyInit(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Kolor czyszc�cy (wype�nienia okna) ustawiono na czarny

}
/*************************************************************************************/

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
    pix2angle = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie
    glMatrixMode(GL_PROJECTION);
    // Prze��czenie macierzy bie��cej na macierz projekcji

    glLoadIdentity();
    // Czyszcznie macierzy bie��cej

    gluPerspective(70, 1.0, 1.0, 100.0);
    // Ustawienie parametr�w dla rzutu perspektywicznego


    if (horizontal <= vertical)
        glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

    else
        glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
    // Ustawienie wielko�ci okna okna widoku (viewport) w zale�no�ci
    // relacji pomi�dzy wysoko�ci� i szeroko�ci� okna

    glMatrixMode(GL_MODELVIEW);
    // Prze��czenie macierzy bie��cej na macierz widoku modelu  

    glLoadIdentity();
    // Czyszczenie macierzy bie��cej
}

/*************************************************************************************/

// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli



void main(void)
{

    //tworzenie jajka
    unsigned int N;
    std::cout << "Podaj N: "; 
    std::cin >> N;
    std::cout << "Z - zmiana obrotow" << std::endl;
    std::cout << "-/+ - zoom" << std::endl;
    std::cout << "R - reset kamery" << std::endl;
    egg = new Egg{N, cords,1.};
    egg->set_spin_theta(false, false, false);

    //tworzenie jajka-satelity
    cords[0] += 6.;
    cords[1] += 2.;
    sat_egg = new Egg{ N / 3, cords, 0.3 };
    sat_egg->set_spin_theta(false, true, false);

    srand(time(NULL));
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(300, 300);

    glutCreateWindow("Uk�ad wsp�rz�dnych 3-D");

    glutDisplayFunc(RenderScene);
    // Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn�
    // (callback function).  Bedzie ona wywo�ywana za ka�dym razem
    // gdy zajdzie potrzba przeryswania okna 

    glutIdleFunc(spin);

    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);
    //glutPassiveMotionFunc(Motion);
    glutKeyboardFunc(keys);

    glutReshapeFunc(ChangeSize);
    // Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn�
    // zazmiany rozmiaru okna      
    MyInit();
    // Funkcja MyInit() (zdefiniowana powy�ej) wykonuje wszelkie
    // inicjalizacje konieczne  przed przyst�pieniem do renderowania
    glEnable(GL_DEPTH_TEST);
    // W��czenie mechanizmu usuwania powierzchni niewidocznych

    glutMainLoop();
    // Funkcja uruchamia szkielet biblioteki GLUT
}

/*************************************************************************************/