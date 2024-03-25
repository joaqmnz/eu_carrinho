#pragma once
#include <math.h>

struct vetor3
{
    float x, y, z;
    vetor3()
    {
        x = 0.0;
        y = 0.0;
        z = 0.0;
    }
    vetor3(float x_, float y_, float z_)
    {
        x = x_;
        y = y_;
        z = z_;
    }

    vetor3 operator+(const vetor3 &v) const
    {
        return vetor3(x + v.x, y + v.y, z + v.z);
    }
    
    vetor3 operator*(float k)
    {
        return vetor3(x * k, y * k, z * k);
    }

    float magnitude()
    {
        return sqrt( (x * x) + (y * y) + (z * z) );
    }

    void normaliza()
    {
        float mag = magnitude();

        x = x / mag;
        y = y / mag;
        z = z / mag;
    }

    vetor3 produtoVetorial(vetor3 v)
    {
        return vetor3( (y * v.z) - (z * v.y), (z * v.x) - (x * v.z), (x * v.y) - (y * v.x) );
    }
};