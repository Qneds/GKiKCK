/*************************************************************************************/
// Najprostszy program wykorzystuj�cy funkcje biblioteki OpenGL
/*************************************************************************************/
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include "Egg.h"
#include <iostream>
#include <time.h>

/*************************************************************************************/

typedef float poin3d[3];

poin3d cords{ 0., 0., 0. };
bool rot = false;

Egg* egg, *sat_egg;
// Funkcaja okre�laj�ca, co ma by� rysowane
// (zawsze wywo�ywana, gdy trzeba przerysowa� scen�)
void RenderScene(void)
{

    
    glMatrixMode(GL_MODELVIEW); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Czyszczenie okna aktualnym kolorem czyszcz�cym

    glLoadIdentity();
    // Czyszczenie macierzy bie��cej

    glTranslated(0, -4, 0);
    glRotated(-35.0, 1., 0., 0.);

    egg->draw();
    glLoadIdentity();

    glTranslated(0, -4, 0);
    glRotated(-35.0, 1., 0., 0.);
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

    GLfloat AspectRatio;
    // Deklaracja zmiennej AspectRatio  okre�laj�cej proporcj�
    // wymiar�w okna 
    if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0
        vertical = 1;
    glViewport(0, 0, horizontal, vertical);
    // Ustawienie wielko�ciokna okna widoku (viewport)
    // W tym przypadku od (0,0) do (horizontal, vertical)  
    glMatrixMode(GL_PROJECTION);
    // Prze��czenie macierzy bie��cej na macierz projekcji 
    glLoadIdentity();
    // Czyszcznie macierzy bie��cej            
    AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
    // Wyznaczenie wsp�czynnika  proporcji okna
    // Gdy okno nie jest kwadratem wymagane jest okre�lenie tak zwanej
    // przestrzeni ograniczaj�cej pozwalaj�cej zachowa� w�a�ciwe
    // proporcje rysowanego obiektu.
    // Do okreslenia przestrzeni ograniczj�cej s�u�y funkcja
    // glOrtho(...)            
    if (horizontal <= vertical)

        glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);
    else

        glOrtho(-7.5 * AspectRatio, 7.5 * AspectRatio, -7.5, 7.5, 10.0, -10.0);
    glMatrixMode(GL_MODELVIEW);
    // Prze��czenie macierzy bie��cej na macierz widoku modelu                                   

    glLoadIdentity();
    // Czyszcenie macierzy bie��cej
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
    egg = new Egg{N, cords,1.};
    egg->set_spin_theta(false, true, false);

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