/*************************************************************************************/
// Najprostszy program wykorzystuj¹cy funkcje biblioteki OpenGL
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
// Funkcaja okreœlaj¹ca, co ma byæ rysowane
// (zawsze wywo³ywana, gdy trzeba przerysowaæ scenê)
void RenderScene(void)
{

    
    glMatrixMode(GL_MODELVIEW); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Czyszczenie okna aktualnym kolorem czyszcz¹cym

    glLoadIdentity();
    // Czyszczenie macierzy bie¿¹cej

    glTranslated(0, -4, 0);
    glRotated(-35.0, 1., 0., 0.);

    egg->draw();
    glLoadIdentity();

    glTranslated(0, -4, 0);
    glRotated(-35.0, 1., 0., 0.);
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

    GLfloat AspectRatio;
    // Deklaracja zmiennej AspectRatio  okreœlaj¹cej proporcjê
    // wymiarów okna 
    if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0
        vertical = 1;
    glViewport(0, 0, horizontal, vertical);
    // Ustawienie wielkoœciokna okna widoku (viewport)
    // W tym przypadku od (0,0) do (horizontal, vertical)  
    glMatrixMode(GL_PROJECTION);
    // Prze³¹czenie macierzy bie¿¹cej na macierz projekcji 
    glLoadIdentity();
    // Czyszcznie macierzy bie¿¹cej            
    AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
    // Wyznaczenie wspó³czynnika  proporcji okna
    // Gdy okno nie jest kwadratem wymagane jest okreœlenie tak zwanej
    // przestrzeni ograniczaj¹cej pozwalaj¹cej zachowaæ w³aœciwe
    // proporcje rysowanego obiektu.
    // Do okreslenia przestrzeni ograniczj¹cej s³u¿y funkcja
    // glOrtho(...)            
    if (horizontal <= vertical)

        glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);
    else

        glOrtho(-7.5 * AspectRatio, 7.5 * AspectRatio, -7.5, 7.5, 10.0, -10.0);
    glMatrixMode(GL_MODELVIEW);
    // Prze³¹czenie macierzy bie¿¹cej na macierz widoku modelu                                   

    glLoadIdentity();
    // Czyszcenie macierzy bie¿¹cej
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

    glutCreateWindow("Uk³ad wspó³rzêdnych 3-D");

    glutDisplayFunc(RenderScene);
    // Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹
    // (callback function).  Bedzie ona wywo³ywana za ka¿dym razem
    // gdy zajdzie potrzba przeryswania okna 

    glutIdleFunc(spin);
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