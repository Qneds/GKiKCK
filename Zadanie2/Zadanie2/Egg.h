#pragma once
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>

class Egg
{
public:
	typedef float point3d[3];
	Egg(unsigned N, const point3d cords, float scale);
	~Egg();


	enum EState {
		e_points,
		e_lines,
		e_triangles
	};


	
	void draw();
	void spin_egg();
	void set_state(EState s);
	void set_spin_theta(bool x, bool y, bool z);


private:
	point3d** m_points;
	point3d** m_colors;
	point3d m_cords;
	point3d m_spin { 0.0, 0.0, 0.0 };

	unsigned m_N;
	float m_dist;
	float m_scale_val;

	
	

	bool m_spinning[3]{};

	EState m_state = e_lines;

	void generate_points();
	int convert_state();
	float rand_color();

};

