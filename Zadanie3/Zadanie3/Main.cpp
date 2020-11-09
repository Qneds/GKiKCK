/*************************************************************************************/
// Najprostszy program wykorzystuj¹cy funkcje biblioteki OpenGL
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

//do odróznienia obrotów
bool rot = false;

static GLfloat viewer[] = { 0.0, 0.0, 20.0 };

Egg* egg, *sat_egg;
MyMouse g_mouse;
Camera g_camera;




// Funkcaja okreœlaj¹ca, co ma byæ rysowane
// (zawsze wywo³ywana, gdy trzeba przerysowaæ scenê)
void RenderScene(void)
{

    
    glMatrixMode(GL_MODELVIEW); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Czyszczenie okna aktualnym kolorem czyszcz¹cym

    glLoadIdentity();
    // Czyszczenie macierzy bie¿¹cej
    if (g_mouse.get_state() & MyMouse::e_left_button)
    {
        g_camera.add_angles(g_ang_speed * pix2angle * g_mouse.get_delta_x(), g_ang_speed * pix2angle * g_mouse.get_delta_y());
    }

    g_camera.set_matrix();
    
                               

    //glRotatef(theta, 0.0, 1.0, 0.0);  //obrót obiektu o nowy k¹t


    //glTranslated(0, -4, 0);
    //glRotated(-35.0, 1., 0., 0.);

    egg->draw();
    //glLoadIdentity();

    //glTranslated(0, -4, 0);
    //glRotated(-35.0, 1., 0., 0.);
    sat_egg->draw();
    
   

    glFlush();
    // Przekazanie poleceñ rysuj¹cych do wykonania


    glutSwapBuffers();
    //
}
/*************************************************************************************/


//funkcja wywo³uj¹ca obroty jajek
void spin() {

    egg->spin_egg();
    sat_egg->spin_egg();
}



// Funkcja "bada" stan myszy i ustawia wartoœci odpowiednich zmiennych globalnych
void Mouse(int btn, int state, int x, int y)
{

    g_mouse.update(btn, state, x, y);

}
/*************************************************************************************/

// Funkcja "monitoruje" po³o¿enie kursora myszy i ustawia wartoœci odpowiednich
// zmiennych globalnych
void Motion(GLsizei x, GLsizei y)
{

    g_mouse.update_pos(x, y);

    glutPostRedisplay();     // przerysowanie obrazu sceny
}
/*************************************************************************************/


//funkcja do ob³ugi klawiszy
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


// Funkcja ustalaj¹ca stan renderowania
void MyInit(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Kolor czyszc¹cy (wype³nienia okna) ustawiono na czarny

}
/*************************************************************************************/

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
    pix2angle = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie
    glMatrixMode(GL_PROJECTION);
    // Prze³¹czenie macierzy bie¿¹cej na macierz projekcji

    glLoadIdentity();
    // Czyszcznie macierzy bie¿¹cej

    gluPerspective(70, 1.0, 1.0, 100.0);
    // Ustawienie parametrów dla rzutu perspektywicznego


    if (horizontal <= vertical)
        glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

    else
        glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
    // Ustawienie wielkoœci okna okna widoku (viewport) w zale¿noœci
    // relacji pomiêdzy wysokoœci¹ i szerokoœci¹ okna

    glMatrixMode(GL_MODELVIEW);
    // Prze³¹czenie macierzy bie¿¹cej na macierz widoku modelu  

    glLoadIdentity();
    // Czyszczenie macierzy bie¿¹cej
}

/*************************************************************************************/

// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli



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

    glutCreateWindow("Uk³ad wspó³rzêdnych 3-D");

    glutDisplayFunc(RenderScene);
    // Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹
    // (callback function).  Bedzie ona wywo³ywana za ka¿dym razem
    // gdy zajdzie potrzba przeryswania okna 

    glutIdleFunc(spin);

    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);
    //glutPassiveMotionFunc(Motion);
    glutKeyboardFunc(keys);

    glutReshapeFunc(ChangeSize);
    // Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹
    // zazmiany rozmiaru okna      
    MyInit();
    // Funkcja MyInit() (zdefiniowana powy¿ej) wykonuje wszelkie
    // inicjalizacje konieczne  przed przyst¹pieniem do renderowania
    glEnable(GL_DEPTH_TEST);
    // W³¹czenie mechanizmu usuwania powierzchni niewidocznych

    glutMainLoop();
    // Funkcja uruchamia szkielet biblioteki GLUT
}

/*************************************************************************************/