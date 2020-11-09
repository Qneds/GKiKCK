#pragma once
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
class Camera
{
public:
	Camera();
	Camera(float az_ang, float el_ang, float distance, GLfloat point_to_look[3]);

	void set_angles(float az_ang, float el_ang);
	void add_angles(float az_ang, float el_ang);
	void set_distance(float dist);
	void add_distance(float dist);
	void reset();
	void update_pos();
	void set_matrix();


private:

	float m_az_angle, m_el_angle, m_distance;
	float m_reset_az_ang, m_reset_el_ang, m_reset_distance;
	GLfloat m_pos[3];
	GLfloat m_look_at[3];
	float vec_up = -1.f;

	void crossProduct(float vect_A[], float  vect_B[], float  cross_P[]);
	float length_of_vec(float vec[]);
	void normalize_angles();

};

