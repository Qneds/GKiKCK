/*************************************************************************************/
// Trochê bardziej skomplikowany program wykorzystuj¹cy funkcje biblioteki OpenGL
/*************************************************************************************/
#define NOMINMAX
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <ios>  
#include <limits>
/*************************************************************************************/


/*************************************************************************************/
// Zmienne sta³e i globalne
/*************************************************************************************/

unsigned int g_levels;
constexpr unsigned int MAX_COLOR_RAND = 1000;
int DEF_PARAM = -1;

/*************************************************************************************/

using namespace std;

typedef float point2[2];

inline float randTo1f(unsigned int max) {
    return rand() % max/ (float) max;
}


void draw(point2 lg, point2 pd, unsigned int lvl) {



    //d³ugoœci boków obecnego kwadratu
    float sideX{ (pd[0] - lg[0]) };
    float sideY{ (lg[1] - pd[1]) };
   

   
    //je¿eli dotrzeliœmy do pozimu zerowego to rysujemy pojedyñczy kwadarat
    if (lvl == 0)
    {
       

        //obliczanie perturbacji
        float offset_x = ((rand() % DEF_PARAM) - DEF_PARAM / 2.f)/(float)(g_levels * sqrt(g_levels));
        float offset_y = ((rand() % DEF_PARAM) - DEF_PARAM / 2.f)/(float)(g_levels * sqrt(g_levels));

        lg[0] += offset_x;
        pd[0] += offset_x;
        lg[1] += offset_y;
        pd[1] += offset_y;



        glBegin(GL_POLYGON);

        glColor3f(randTo1f(MAX_COLOR_RAND), randTo1f(MAX_COLOR_RAND), randTo1f(MAX_COLOR_RAND));
        glVertex2f(lg[0], lg[1]);
        glColor3f(randTo1f(MAX_COLOR_RAND), randTo1f(MAX_COLOR_RAND), randTo1f(MAX_COLOR_RAND));
        glVertex2f(lg[0], pd[1]);
        glColor3f(randTo1f(MAX_COLOR_RAND), randTo1f(MAX_COLOR_RAND), randTo1f(MAX_COLOR_RAND));
        glVertex2f(pd[0], pd[1]);
        glColor3f(randTo1f(MAX_COLOR_RAND), randTo1f(MAX_COLOR_RAND), randTo1f(MAX_COLOR_RAND));
        glVertex2f(pd[0], lg[1]);

        glEnd();

        

        return;
    }


    //boki nowych kwadratów
    float side_sX{ sideX / 3 };
    float side_sY{ sideY / 3 };

   

    for (int i{ 0 }; i < 3; i++) {
        for (int j{ 0 }; j < 3; j++) {


            //nigdy nie rysyjemy œrodkowego podkwadratu
            if (i == 1 && j == 1)
                continue;

            //wspó³rzêdne wierzcho³ow podkwadratu
            point2 new_lg{ lg[0] + i * side_sX, lg[1] - j * side_sY };
            point2 new_pd{ lg[0] + i * side_sX + side_sX, lg[1] - j * side_sY - side_sY };


            //wywo³anie funkcji na podkwadracie w celu podcia³u na kolejne podkwadaraty
            draw(new_lg, new_pd, lvl-1);

        }
    }

    


}


void RenderScene(void)

{

    glClear(GL_COLOR_BUFFER_BIT);
    // Czyszczenie okna aktualnym kolorem czyszcz¹cym


    //kwadrat pocz¹tkowy
    point2 lg{ -50.f, 50.f };
    point2 pd{ 50.f, -50.f };


    //uruchumienie procedury rysowania dywanu Sierpinskiego
    draw(lg, pd, g_levels);

    glFlush();
    // Przekazanie poleceñ rysuj¹cych do wykonania

}

/*************************************************************************************/

// Funkcja ustalaj¹ca stan renderowania
void MyInit(void)

{

    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    // Kolor okna wnêtrza okna - ustawiono na szary

}

/*************************************************************************************/

// Funkcja s³u¿¹ca do kontroli zachowania proporcji rysowanych obiektów
// niezale¿nie od rozmiarów okna graficznego



void ChangeSize(GLsizei horizontal, GLsizei vertical)

// Parametry horizontal i vertical (szerokoœæ i wysokoœæ okna) s¹
// przekazywane do funkcji za ka¿dym razem, gdy zmieni siê rozmiar okna

{

    GLfloat AspectRatio;

    // Deklaracja zmiennej AspectRatio okreœlaj¹cej proporcjê wymiarów okna



    if (vertical == 0)
        // Zabezpieczenie pzred dzieleniem przez 0

        vertical = 1;


    glViewport(0, 0, horizontal, vertical);
    // Ustawienie wielkoœciokna okna urz¹dzenia (Viewport)
    // W tym przypadku od (0,0) do (horizontal, vertical)


    glMatrixMode(GL_PROJECTION);
    // Okreœlenie uk³adu wspó³rzêdnych obserwatora

    glLoadIdentity();
    // Okreœlenie przestrzeni ograniczaj¹cej

    AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
    // Wyznaczenie wspó³czynnika proporcji okna

    // Gdy okno na ekranie nie jest kwadratem wymagane jest
    // okreœlenie okna obserwatora.
    // Pozwala to zachowaæ w³aœciwe proporcje rysowanego obiektu
    // Do okreœlenia okna obserwatora s³u¿y funkcja glOrtho(...)



    if (horizontal <= vertical)

        glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);

    else

        glOrtho(-100.0 * AspectRatio, 100.0 * AspectRatio, -100.0, 100.0, 1.0, -1.0);

    glMatrixMode(GL_MODELVIEW);
    // Okreœlenie uk³adu wspó³rzêdnych    

    glLoadIdentity();

}

/*************************************************************************************/

// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli
int main(void)
{

    cout << "Podaj ilosc poziomow: ";
    cin >> g_levels;
    
    do {

        cout << "Podaj poziom deformacji( >0, 0 -> wawrtosc domyslna (6) ): ";
        cin >> DEF_PARAM;

    } while (DEF_PARAM < 0);

    if (!DEF_PARAM) DEF_PARAM = 6;


    srand(time(NULL));

    glutInitDisplayMode(GLUT_SINGLE| GLUT_RGBA);

    glutCreateWindow("Dywan Sierpinskiego");

    glutDisplayFunc(RenderScene);

    glutReshapeFunc(ChangeSize);

    MyInit();

    glutMainLoop();

  
}


