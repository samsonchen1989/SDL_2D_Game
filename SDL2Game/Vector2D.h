#ifndef VECTOR_2D_H
#define VECTOR_2D_H

#include <math.h>

class Vector2D
{
public:
    //Todo: use template here to replace float and
    //we will not be confused by convet in int/float/double
    Vector2D(float x = 0, float y = 0) : m_x(x), m_y(y) {}
    Vector2D(const Vector2D& v) : m_x(v.m_x), m_y(v.m_y) {}

    float getX() { return m_x; }
    float getY() { return m_y; }

    void setX(float x) { m_x = x; }
    void setY(float y) { m_y = y; }

    float length() { return sqrt(m_x * m_x + m_y * m_y); }

    void normalize()
    {
        float len = length();
        if (len > 0) {
            (*this) *= (1 / len);
        }
    }

    Vector2D& operator=(const Vector2D& v)
    {
        if (this == &v) {
            return *this;
        }

        m_x = v.m_x;
        m_y = v.m_y;

        return *this;
    }

    Vector2D& operator+=(const Vector2D& v)
    {
        m_x += v.m_x;
        m_y += v.m_y;

        return *this;
    }

    friend Vector2D operator+(Vector2D leftV, const Vector2D& rightV)
    {
        return (leftV += rightV);
    }

    Vector2D operator*(float scalar)
    {
        return Vector2D(m_x * scalar, m_y * scalar);
    }

    Vector2D& operator*=(float scalar)
    {
        m_x *= scalar;
        m_y *= scalar;

        return *this;
    }

    Vector2D operator/(float scalar)
    {
        return Vector2D(m_x / scalar, m_y / scalar);
    }

    Vector2D& operator/=(float scalar)
    {
        m_x /= scalar;
        m_y /= scalar;

        return *this;
    }

    Vector2D& operator-=(const Vector2D& v)
    {
        m_x -= v.m_x;
        m_y -= v.m_y;

        return *this;
    }

    friend Vector2D operator-(Vector2D leftV, const Vector2D& rightV)
    {
        return (leftV -= rightV);
    }

    float m_x;
    float m_y;
};

#endif