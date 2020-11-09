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


// STA£E
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
	// Czyszczenie okna aktualnym kolorem czyszcz¹cym

	glLoadIdentity();
	// Czyszczenie macierzy bie¿¹cej

	if (g_mouse.get_state() & MyMouse::e_left_button)
	{
		g_camera.add_angles(g_ang_speed * pix2angle * g_mouse.get_delta_x(), g_ang_speed * pix2angle * g_mouse.get_delta_y());
	}
	// zmiana pozycji kameru

	g_camera.set_matrix();
	// ustawienie macierzy lookat

	g_wei.draw();
	glFlush();
	// Przekazanie poleceñ rysuj¹cych do wykonania

	glutSwapBuffers();
}
/*********************************************************************/

// Funkcja "bada" stan myszy i ustawia wartoœci odpowiednich zmiennych globalnych
void Mouse(int btn, int state, int x, int y)
{

	g_mouse.update(btn, state, x, y);
	// aktualizacja stanu myszki

}
/*************************************************************************************/

// Funkcja "monitoruje" po³o¿enie kursora myszy i ustawia wartoœci odpowiednich
// zmiennych globalnych
void Motion(GLsizei x, GLsizei y)
{

	g_mouse.update_pos(x, y);
	// aktualizacja pozycji myszki
	glutPostRedisplay();     // przerysowanie obrazu sceny


}
/*************************************************************************************/

void keys(unsigned char key, int x, int y) {

	// przybli¿enie kamry i zwiêkszenie szczegó³owoœci p³aszczyzny
	if (key == '+') {
		g_camera.add_distance(-G_DISTANCE_PER_SCROLL);
		if (g_wei.get_inner_size() >= 50) {
			g_wei.set_num_of_rot(g_wei.get_num_of_rot() + 1);
		}
		g_wei.set_size(g_wei.get_inner_size() + 25);


	}

	// oddalenie kamery i zmniejszenie szczegó³owoœci p³aszczyzny
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

	// zmiana sposobu wyœwietlania p³aszczyzny
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
	// Kolor czyszc¹cy (wype³nienia okna) ustawiono na czarny
}
/********************************************************************
*****************/
// Funkcja ma za zadanie utrzymanie sta³ych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokoœæ i szerokoœæ okna) s¹
// przekazywane do funkcji za ka¿dym razem gdy zmieni siê rozmiar okna.

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
/********************************************************************
*****************/
// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli

int main()
{

	std::cout << "Z - zmiana trybu wyswietlania" << std::endl;
	std::cout << "-/+ - zoom" << std::endl;
	std::cout << "R - reset kamery" << std::endl;

	srand(time(NULL));
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(300, 300);

	glutCreateWindow("Uk³ad wspó³rzêdnych 3-D");

	glutDisplayFunc(RenderScene);
	// Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹
	// (callback function).  Bedzie ona wywo³ywana za ka¿dym razem
	// gdy zajdzie potrzba przeryswania okna 

	// glutIdleFunc(spin);

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
/********************************************************************
*****************/