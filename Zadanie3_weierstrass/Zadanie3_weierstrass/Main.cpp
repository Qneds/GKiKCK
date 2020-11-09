/*********************************************************************/
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "Camera.h"
#include "Mouse.h"
#include "Weierstrass.h"
#include <time.h>
#include <iostream>
/*********************************************************************/


// STA�E
/*********************************************************************/
static GLfloat pix2angle;     // przelicznik pikseli na stopnie
float g_ang_speed = 0.1f;
constexpr float G_DISTANCE_PER_SCROLL = 1.;

MyMouse g_mouse;
Camera g_camera;
Weierstrass g_wei;

typedef float point3[3];
typedef float point2[2];
/*********************************************************************/

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
	// zmiana pozycji kameru

	g_camera.set_matrix();
	// ustawienie macierzy lookat

	g_wei.draw();
	glFlush();
	// Przekazanie polece� rysuj�cych do wykonania

	glutSwapBuffers();
}
/*********************************************************************/

// Funkcja "bada" stan myszy i ustawia warto�ci odpowiednich zmiennych globalnych
void Mouse(int btn, int state, int x, int y)
{

	g_mouse.update(btn, state, x, y);
	// aktualizacja stanu myszki

}
/*************************************************************************************/

// Funkcja "monitoruje" po�o�enie kursora myszy i ustawia warto�ci odpowiednich
// zmiennych globalnych
void Motion(GLsizei x, GLsizei y)
{

	g_mouse.update_pos(x, y);
	// aktualizacja pozycji myszki
	glutPostRedisplay();     // przerysowanie obrazu sceny


}
/*************************************************************************************/

void keys(unsigned char key, int x, int y) {

	// przybli�enie kamry i zwi�kszenie szczeg�owo�ci p�aszczyzny
	if (key == '+') {
		g_camera.add_distance(-G_DISTANCE_PER_SCROLL);
		if (g_wei.get_inner_size() >= 50) {
			g_wei.set_num_of_rot(g_wei.get_num_of_rot() + 1);
		}
		g_wei.set_size(g_wei.get_inner_size() + 25);


	}

	// oddalenie kamery i zmniejszenie szczeg�owo�ci p�aszczyzny
	if (key == '-') {
		g_camera.add_distance(G_DISTANCE_PER_SCROLL);
		if (g_wei.get_inner_size() > 50) {
			g_wei.set_num_of_rot(g_wei.get_num_of_rot() - 1);
		}
		g_wei.set_size(g_wei.get_inner_size() - 25);
	}

	// reset kamery
	if (key == 'r') {
		g_camera.reset();
		g_wei.set_num_of_rot(7);
		g_wei.set_size(100);
	}

	// zmiana sposobu wy�wietlania p�aszczyzny
	if (key == 'z') {
		
		Weierstrass::EState s = g_wei.get_state();

		if (s == g_wei.e_points) {
			g_wei.set_state(g_wei.e_lines);
		}
		if (s == g_wei.e_lines) {
			g_wei.set_state(g_wei.e_polygons);
		}
		if (s == g_wei.e_polygons) {
			g_wei.set_state(g_wei.e_points);
		}

	}


	RenderScene(); // przerysowanie obrazu sceny
}

void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszc�cy (wype�nienia okna) ustawiono na czarny
}
/********************************************************************
*****************/
// Funkcja ma za zadanie utrzymanie sta�ych proporcji rysowanych
// w przypadku zmiany rozmiar�w okna.
// Parametry vertical i horizontal (wysoko�� i szeroko�� okna) s�
// przekazywane do funkcji za ka�dym razem gdy zmieni si� rozmiar okna.

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
/********************************************************************
*****************/
// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli

int main()
{

	std::cout << "Z - zmiana trybu wyswietlania" << std::endl;
	std::cout << "-/+ - zoom" << std::endl;
	std::cout << "R - reset kamery" << std::endl;

	srand(time(NULL));
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(300, 300);

	glutCreateWindow("Uk�ad wsp�rz�dnych 3-D");

	glutDisplayFunc(RenderScene);
	// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn�
	// (callback function).  Bedzie ona wywo�ywana za ka�dym razem
	// gdy zajdzie potrzba przeryswania okna 

	// glutIdleFunc(spin);

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
/********************************************************************
*****************/