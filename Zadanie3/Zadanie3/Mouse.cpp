#include "Mouse.h"
#include <iostream>
using namespace std;

MyMouse::MyMouse()
{
}

// aktualizacja stanu myszy
void MyMouse::update(int button, int state, int x, int y)
{

    m_x_old_pos = x;
    m_y_old_pos = y;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        m_state = m_state | e_left_button;
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
    {
        m_state = m_state & (~e_left_button);
    }

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        m_state = m_state | e_right_button;
    }

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
    {
        m_state = m_state & (~e_right_button);
    }

}

// aktualizacja pozycji myszy
void MyMouse::update_pos(int x, int y)
{

    m_x_pos = x;
    m_y_pos = y;

    m_delta_x = m_x_pos - m_x_old_pos;
    m_delta_y = m_y_pos - m_y_old_pos;

    m_x_old_pos = m_x_pos;
    m_y_old_pos = m_y_pos;
}

// zwrócenice dystansu w osi X pomiêdzy aktualn¹ pozycj¹ a poprzedni¹ pozycj¹
int MyMouse::get_delta_x()
{
    int t = m_delta_x;
    m_delta_x = 0;
    return t;
}


// zwrócenice dystansu w osi Y pomiêdzy aktualn¹ pozycj¹ a poprzedni¹ pozycj¹
int MyMouse::get_delta_y()
{
    int t = m_delta_y;
    m_delta_y = 0;
    return t;
}

// zwrócenie stanu myszki
int MyMouse::get_state()
{
    return m_state;
}


