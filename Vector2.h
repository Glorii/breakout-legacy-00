/*
 * Vector2.h
 *
 *  Created on: 2015年1月6日
 *      Author: lenovo
 */

#ifndef VECTOR2_H_
#define VECTOR2_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include <math.h>

class Vector2 {
public:
	Vector2(float t_ptCenter_x, float t_ptCenter_y, float t_nX = 0.0, float t_nY = 0.0);
	virtual ~Vector2();

	void updatefunc(float t_nX = 0.0, float t_nY = 0.0);
    static float Angle(Vector2 t_vec2_01, Vector2 t_vec2_02);
    void normalize();
    static int LeftOrRight(Vector2 t_vec2_01, Vector2 t_vec2_02);

    float getNDelX() const
    {
        return m_nDelX;
    }

    float getNDelY() const
    {
        return m_nDelY;
    }

    void setNDelX(float m_nDelX)
    {
        this->m_nDelX = m_nDelX;
    }

    void setNDelY(float m_nDelY)
    {
        this->m_nDelY = m_nDelY;
    }

private:
    float m_nX;
    float m_nY;
    float m_nDelX;
    float m_nDelY;
    float m_nLen;
    float m_ptCenter_x;
    float m_ptCenter_y;



};

#endif /* VECTOR2_H_ */
