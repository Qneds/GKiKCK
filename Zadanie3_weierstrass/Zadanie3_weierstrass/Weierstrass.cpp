#define _USE_MATH_DEFINES
#define NOMINMAX
#include <Windows.h>
#include "Weierstrass.h"
#include <math.h>


Weierstrass::Weierstrass(int s, int rot)
{

	m_size = s;
	m_rot = rot;
	m_inner_size = s;
	alocate();
	generate_colors();

}

Weierstrass::Weierstrass() : Weierstrass(100, 7)
{
}

Weierstrass::~Weierstrass()
{
	dealocate();
}

// Ustawienie szegó³owoœci funcki Weierstrass
void Weierstrass::set_size(int s)
{
	dealocate();
	m_inner_size = s;
	if (m_inner_size < 50.)
		m_size = 50.f;
	else
		m_size = m_inner_size;
	alocate();
	generate_colors();
}



void Weierstrass::draw()
{


	glTranslatef(-5.f, 0.f, 0.f);

	switch (m_state)
	{
	case e_polygons:

		for (int j = 0; j < m_rot - 1; j++) {
			for (int i = 0; i < m_size - 1; i++) {
				glBegin(GL_POLYGON);
				glColor3fv(m_colors[j][i]);
				glVertex3fv(m_tab_3d[j][i]);

				glColor3fv(m_colors[j][i + 1]);
				glVertex3fv(m_tab_3d[j][i + 1]);

				glColor3fv(m_colors[j + 1][i + 1]);
				glVertex3fv(m_tab_3d[j + 1][i + 1]);

				glColor3fv(m_colors[j + 1][i]);
				glVertex3fv(m_tab_3d[j + 1][i]);
				glEnd();
			}
		}
		break;

	case e_lines:


		for (int j = 0; j < m_rot - 1; j++) {
			for (int i = 0; i < m_size - 1; i++) {
				glColor3f(0.0f, 1.0f, 0.0f);
				glBegin(GL_LINES);
				glVertex3fv(m_tab_3d[j][i]);
				glVertex3fv(m_tab_3d[j][i + 1]);
				glEnd();


				glBegin(GL_LINES);
				glVertex3fv(m_tab_3d[j][i]);
				glVertex3fv(m_tab_3d[j + 1][i]);
				glEnd();
			}
		}

		for (int i = 0; i < m_size - 1; i++) {
			glColor3f(0.0f, 1.0f, 0.0f);
			glBegin(GL_LINES);
			glVertex3fv(m_tab_3d[m_rot - 1][i]);
			glVertex3fv(m_tab_3d[m_rot - 1][i + 1]);
			glEnd();
		}

		break;


	case e_points:

		for (int j = 0; j < m_rot; j++) {
			for (int i = 0; i < m_size - 1; i++) {
				glColor3f(1.0f, 1.0f, 0.0f);
				glBegin(GL_POINTS);
				glVertex3fv(m_tab_3d[j][i]);
				glEnd();
			}
		}
		break;

	}

}

// ustawienie iloœci obrotów funkcji Weierstrassa wokó³ osi X
void Weierstrass::set_num_of_rot(unsigned int r)
{
	dealocate();
	m_rot = r;
	alocate();
	generate_colors();
}

// wygenerowanie dwuwymiarowej funcji Weierstrassa
void Weierstrass::generate_2d()
{
	float x = 0;
	float s = 1./m_size;
	float scale = 10;
	for (int i = 0; i < m_size; i++)
	{
		m_tab_2d[i][0] = x * scale;
		m_tab_2d[i][1] = scale * weierstrass(x);
		x += s;
	}
}

// obracanie dwuwymiarowej funkcji Weierstrassa wokó³ osi X w trójwymiarowej przestrzeni
void Weierstrass::generate_3d()
{

	
	float x, y, z;
	float delat_theta = M_PI / (m_rot - 1);
	float theta = 0.f;

	for (int j = 0; j < m_rot; j++, theta += delat_theta)
	{

		for (int i = 0; i < m_size; i++)
		{
			m_tab_3d[j][i][0] = m_tab_2d[i][0]; 
			m_tab_3d[j][i][1] = m_tab_2d[i][1];
			m_tab_3d[j][i][2] = 0;


			
			z = m_tab_3d[j][i][1] * sin(theta) + m_tab_3d[j][i][2] * cos(theta);
			y = m_tab_3d[j][i][1] * cos(theta) - m_tab_3d[j][i][2] * sin(theta);
			x = m_tab_3d[j][i][0];

			m_tab_3d[j][i][0] = x;
			m_tab_3d[j][i][1] = y;
			m_tab_3d[j][i][2] = z;

		}
	}


}


// losowanie kolorów dla punktów
void Weierstrass::generate_colors()
{

	for (int j = 0; j < m_rot; j++) {
		for (int i = 0; i < m_size; i++) {
			m_colors[j][i][0] = rand_color();
			m_colors[j][i][1] = rand_color();
			m_colors[j][i][2] = rand_color();
		}
	}

}

// liczenie wartoœci funckji Weierstrassa dla danego x
float Weierstrass::weierstrass(float x)
{
	float sum = 0;
	for (int k = 1; k <= 20; k++)
	{
		sum += sin(M_PI * pow(k, a) * x) / (M_PI * pow(k, a));
		
	}
	return sum;
}

// alokacja potrzebnej pamiêci
void Weierstrass::alocate()
{
	m_tab_2d = new point2[m_size];
	m_tab_3d = new point3*[m_rot];
	m_colors = new point3*[m_rot];
	for (int i{ 0 }; i < m_rot; i++) {
		m_tab_3d[i] = new point3[m_size];
		m_colors[i] = new point3[m_size];
	}
	generate_2d();
	generate_3d();
}

// dealokacja potrzebnej pamiêci
void Weierstrass::dealocate()
{
	for (int i{ 0 }; i < m_rot; i++) {
		delete[] m_tab_3d[i];
		delete[] m_colors[i];
	}
	delete[] m_tab_2d;
	delete[] m_tab_3d;
	delete[] m_colors;
}


// losowanie koloru z zakresu 0 - 0.999
float Weierstrass::rand_color()
{
	return rand() % 1000 / 1000.;
}
