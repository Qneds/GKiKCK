/*************************************************************************************/
// Troch� bardziej skomplikowany program wykorzystuj�cy funkcje biblioteki OpenGL
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
// Zmienne sta�e i globalne
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



    //d�ugo�ci bok�w obecnego kwadratu
    float sideX{ (pd[0] - lg[0]) };
    float sideY{ (lg[1] - pd[1]) };
   

   
    //je�eli dotrzeli�my do pozimu zerowego to rysujemy pojedy�czy kwadarat
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


    //boki nowych kwadrat�w
    float side_sX{ sideX / 3 };
    float side_sY{ sideY / 3 };

   

    for (int i{ 0 }; i < 3; i++) {
        for (int j{ 0 }; j < 3; j++) {


            //nigdy nie rysyjemy �rodkowego podkwadratu
            if (i == 1 && j == 1)
                continue;

            //wsp�rz�dne wierzcho�ow podkwadratu
            point2 new_lg{ lg[0] + i * side_sX, lg[1] - j * side_sY };
            point2 new_pd{ lg[0] + i * side_sX + side_sX, lg[1] - j * side_sY - side_sY };


            //wywo�anie funkcji na podkwadracie w celu podcia�u na kolejne podkwadaraty
            draw(new_lg, new_pd, lvl-1);

        }
    }

    


}


void RenderScene(void)

{

    glClear(GL_COLOR_BUFFER_BIT);
    // Czyszczenie okna aktualnym kolorem czyszcz�cym


    //kwadrat pocz�tkowy
    point2 lg{ -50.f, 50.f };
    point2 pd{ 50.f, -50.f };


    //uruchumienie procedury rysowania dywanu Sierpinskiego
    draw(lg, pd, g_levels);

    glFlush();
    // Przekazanie polece� rysuj�cych do wykonania

}

/*************************************************************************************/

// Funkcja ustalaj�ca stan renderowania
void MyInit(void)

{

    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    // Kolor okna wn�trza okna - ustawiono na szary

}

/*************************************************************************************/

// Funkcja s�u��ca do kontroli zachowania proporcji rysowanych obiekt�w
// niezale�nie od rozmiar�w okna graficznego



void ChangeSize(GLsizei horizontal, GLsizei vertical)

// Parametry horizontal i vertical (szeroko�� i wysoko�� okna) s�
// przekazywane do funkcji za ka�dym razem, gdy zmieni si� rozmiar okna

{

    GLfloat AspectRatio;

    // Deklaracja zmiennej AspectRatio okre�laj�cej proporcj� wymiar�w okna



    if (vertical == 0)
        // Zabezpieczenie pzred dzieleniem przez 0

        vertical = 1;


    glViewport(0, 0, horizontal, vertical);
    // Ustawienie wielko�ciokna okna urz�dzenia (Viewport)
    // W tym przypadku od (0,0) do (horizontal, vertical)


    glMatrixMode(GL_PROJECTION);
    // Okre�lenie uk�adu wsp�rz�dnych obserwatora

    glLoadIdentity();
    // Okre�lenie przestrzeni ograniczaj�cej

    AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
    // Wyznaczenie wsp�czynnika proporcji okna

    // Gdy okno na ekranie nie jest kwadratem wymagane jest
    // okre�lenie okna obserwatora.
    // Pozwala to zachowa� w�a�ciwe proporcje rysowanego obiektu
    // Do okre�lenia okna obserwatora s�u�y funkcja glOrtho(...)



    if (horizontal <= vertical)

        glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);

    else

        glOrtho(-100.0 * AspectRatio, 100.0 * AspectRatio, -100.0, 100.0, 1.0, -1.0);

    glMatrixMode(GL_MODELVIEW);
    // Okre�lenie uk�adu wsp�rz�dnych    

    glLoadIdentity();

}

/*************************************************************************************/

// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli
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


