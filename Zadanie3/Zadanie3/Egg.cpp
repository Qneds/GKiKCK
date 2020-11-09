#include "Egg.h"
#define _USE_MATH_DEFINES
#include <math.h>

Egg::Egg(unsigned N, const point3d cords, float scale) : m_scale_val{ scale }, m_N{ N }, m_dist{ 1.f / m_N }
{

	m_points = new point3d * [m_N];
	m_colors = new point3d * [m_N];


	for (int i{ 0 }; i < m_N; i++) {

		m_points[i] = new point3d[m_N];
		m_colors[i] = new point3d[m_N];

		if (i < 3) {
			m_cords[i] = cords[i];
		}

	}
	generate_points();
}

Egg::~Egg()
{
	for (int i{ 0 }; i < m_N; i++) {

		delete[] m_points[i];
		delete[] m_colors[i];
	}

	delete m_colors;
	delete[] m_points;

}

void Egg::draw()
{
	
	
	
	//glTranslatef(m_cords[0], m_cords[1], m_cords[2]);


	//ustawianie orientacji i pozycji
	glTranslatef(0., 5., 0.);

	glRotatef(m_spin[0], 1.0, 0.0, 0.0);
	glRotatef(m_spin[1], 0.0, 1.0, 0.0);
	glRotatef(m_spin[2], 0.0, 0.0, 1.0);

	glTranslatef(0., -5., 0.);

	glTranslatef(m_cords[0], m_cords[1], m_cords[2]);

	glScalef(m_scale_val, m_scale_val, m_scale_val);
	//glRotatef(m_rot_arround[0], 1.0, 0.0, 0.0);
	//glRotatef(m_rot_arround[1], 0.0, 1.0, 0.0);
	//glRotatef(m_rot_arround[2], 0.0, 0.0, 1.0);



	glColor3f(1.0f, 1.0f, 1.0f);
	

	
	
	switch (m_state)
	{
	case e_points:

		for (int u{ 0 }; u < m_N; u++) {

			for (int v{ 0 }; v < m_N; v++) {
				glBegin(convert_state());
				glVertex3fv(m_points[u][v]);
				glEnd();
			}
		}
		break;

	case e_lines:
		for (int i = 0; i < m_N; i++) {
			for (int j = 0; j < m_N; j++) {

				// linie pionowe
				glBegin(GL_LINES);
				glColor3f(0.0f, 1.0f, 0.0f);
				glVertex3fv(m_points[i][j]);
				glVertex3fv(m_points[(i + 1) % m_N][j]);
				glEnd();

				//linie ukoœne
				glBegin(GL_LINES);
				if (((j + 1) != m_N) && ((i + 1) != m_N)) {
					glVertex3fv(m_points[i + 1][j]);
					glVertex3fv(m_points[i][j + 1]);
				}
				else {
					if (i > 0) {
						glVertex3fv(m_points[i][j]);
						glVertex3fv(m_points[m_N - i - 1][0]);
					}
				}
				glEnd();

				//linie poziome
				glBegin(GL_LINES);
				glColor3f(0.0f, 1.0f, 0.0f);
				if ((j + 1) == m_N) {
					if (i > 0) {
						glVertex3fv(m_points[m_N - i][0]);
						glVertex3fv(m_points[i][j]);
					}
				}
				else {
					glVertex3fv(m_points[i][j + 1]);
					glVertex3fv(m_points[i][j]);
				}
				glEnd();
			}
		}
		break;

	case e_triangles:

		for (int i = 0; i < m_N; i++) {
			for (int j = 0; j < m_N; j++) {
				if ((j + 1) != m_N) {
					glBegin(GL_TRIANGLES);
					glColor3fv(m_colors[i][j]);
					glVertex3fv(m_points[i][j]);
					glColor3fv(m_colors[(i + 1) % m_N][j]);
					glVertex3fv(m_points[(i + 1) % m_N][j]);
					glColor3fv(m_colors[i][j + 1]);
					glVertex3fv(m_points[i][j + 1]);
					glEnd();

					glBegin(GL_TRIANGLES);
					glColor3fv(m_colors[(i + 1) % m_N][j]);
					glVertex3fv(m_points[(i + 1) % m_N][j]);
					glColor3fv(m_colors[(i + 1) % m_N][j + 1]);
					glVertex3fv(m_points[(i + 1) % m_N][j + 1]);
					glColor3fv(m_colors[i][j + 1]);
					glVertex3fv(m_points[i][j + 1]);
					glEnd();
				}
				else {
					if (i > 0) {
						glBegin(GL_TRIANGLES);
						glColor3fv(m_colors[i][j]);
						glVertex3fv(m_points[i][j]);
						glColor3fv(m_colors[(i + 1) % m_N][j]);
						glVertex3fv(m_points[(i + 1) % m_N][j]);
						glColor3fv(m_colors[m_N - i][0]);
						glVertex3fv(m_points[m_N - i][0]);
						glEnd();
					}
					glBegin(GL_TRIANGLES);
					glColor3fv(m_colors[(i + 1) % m_N][j]);
					glVertex3fv(m_points[(i + 1) % m_N][j]);
					glColor3fv(m_colors[(m_N - i) % m_N][0]);
					glVertex3fv(m_points[(m_N - i) % m_N][0]);
					glColor3fv(m_colors[m_N - i - 1][0]);
					glVertex3fv(m_points[m_N - i - 1][0]);
					glEnd();

				}
			}
		}
		break;

		break;
	}

}


// generowanie chmury punktów
void Egg::generate_points()
{

	float u_off{ 0.}, v_off;

	for (int u{ 0 }; u < m_N; u++) {

		v_off = 0.;

		for (int v{ 0 }; v < m_N; v++ ) {


			float p = ((-90) * powf(u_off, 5) + 225 * powf(u_off, 4) - 270 * powf(u_off, 3) +
				180 * powf(u_off, 2) - 45 * u_off);

			m_points[u][v][0] = p * cos(M_PI * v_off);
			m_points[u][v][1] = 160 * powf(u_off, 4) - 320 * powf(u_off, 3) + 160 * powf(u_off, 2);
			m_points[u][v][2] = p * sin(M_PI * v_off);

			m_colors[u][v][0] = rand_color();
			m_colors[u][v][1] = rand_color();
			m_colors[u][v][2] = rand_color();

			v_off += m_dist;

		}

		u_off += m_dist;
	}

}

int Egg::convert_state()
{
	switch (m_state)
	{
	case Egg::e_points:
		return GL_POINTS;
	case Egg::e_lines:
		return GL_LINES;
	case Egg::e_triangles:
		return GL_TRIANGLES;
	}

}


float Egg::rand_color()
{
	return rand() % 1000 / 1000.;
}

//okreœlenie konta obrotu wokó³ danej osi
void Egg::spin_egg()
{

	if (m_spinning[0])
	{
		m_spin[0] -= 0.5;
		if (m_spin[0] > 360.0) m_spin[0] -= 360.0;
	}

	if (m_spinning[1])
	{
		m_spin[1] -= 0.5;
		if (m_spin[1] > 360.0) m_spin[1] -= 360.0;
	}

	if (m_spinning[2])
	{
		m_spin[2] -= 0.5;
		if (m_spin[2] > 360.0) m_spin[2] -= 360.0;
	}

	glutPostRedisplay(); //odœwie¿enie zawartoœci aktualnego okna
}

void Egg::set_state(EState s)
{
	m_state = s;
}

void Egg::set_spin_theta(bool x, bool y, bool z)
{
	//ustawienie wokó³ których osi ma nastêpowaæ obrót
	m_spinning[0] = x;
	m_spinning[1] = y;
	m_spinning[2] = z;

	m_spin[0] = 0.;
	m_spin[1] = 0.;
	m_spin[2] = 0.;

}


