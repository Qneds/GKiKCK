#pragma once
#include <stdlib.h>
#include <gl/gl.h>
#include <gl/glut.h>


class Weierstrass
{
public:

	enum EState {
		e_points,
		e_lines,
		e_polygons
	};

	typedef float point3[3];
	typedef float point2[2];
	Weierstrass(int s, int rot);
	Weierstrass();
	~Weierstrass();
	void set_size(int s);
	int get_size() { return m_size; }
	int get_inner_size() { return m_inner_size; }
	void draw();
	unsigned int get_num_of_rot() { return m_rot; }
	void set_num_of_rot(unsigned int r);
	void set_state(EState s) { m_state = s; }
	EState get_state() { return m_state; }

private:
	
	void generate_2d();
	void generate_3d();
	void generate_colors();
	float weierstrass(float x);
	void alocate();
	void dealocate();
	float rand_color();
	int m_size;
	int m_inner_size;
	unsigned int m_rot;

	EState m_state = e_polygons;

	float a = 3;
	point2 *m_tab_2d;
	point3 **m_tab_3d;
	point3 **m_colors;
};

