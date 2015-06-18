/*
 * Vector2.cpp
 *
 *  Created on: 2015年1月6日
 *      Author: lenovo
 */

#include "Vector2.h"

Vector2::Vector2(float t_ptCenter_x, float t_ptCenter_y, float t_nX, float t_nY) {
	/*t_nX = 0;
	t_nY = 0;*/

	m_nX=t_nX;
	m_nY=t_nY;
	m_ptCenter_x = t_ptCenter_x;
	m_ptCenter_y = t_ptCenter_y;

}

void Vector2::updatefunc(float t_nX, float t_nY){
	m_nX=t_nX;
	m_nY=t_nY;
	m_nLen=1;

	m_nDelX=m_nX-m_ptCenter_x;
	m_nDelY=m_nY-m_ptCenter_y;
}

float Vector2::Angle(Vector2 t_vec2_01,Vector2 t_vec2_02){
	t_vec2_01.normalize();
	t_vec2_02.normalize();

	return Vector2::LeftOrRight(t_vec2_01,t_vec2_02) * acos(t_vec2_01.getNDelX() * t_vec2_02.getNDelX() + t_vec2_01.getNDelY() * t_vec2_02.getNDelY()) * 180 / M_PI;
}

void Vector2::normalize(){
	m_nLen=sqrtf(m_nDelX * m_nDelX + m_nDelY * m_nDelY);
	m_nDelX=m_nDelX / m_nLen;
	m_nDelY=m_nDelY / m_nLen;
}

int Vector2::LeftOrRight(Vector2 t_vec2_01,Vector2 t_vec2_02){
	return (t_vec2_02.getNDelY() * t_vec2_01.m_nDelX - t_vec2_02.getNDelX() * t_vec2_01.getNDelY()) > 0 ? (-1):(1);
}

Vector2::~Vector2() {
	// TODO Auto-generated destructor stub
}

