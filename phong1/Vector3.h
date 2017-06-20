#pragma once

class Vector3
{
public:
  Vector3(void) 
    : x(0), y(0), z(0)   
  {}
  Vector3(float _x, float _y, float _z)
    : x(_x), y(_y), z(_z)
  {}

  float&  operator() (unsigned int i)         { return  (&x)[i]; }
  float   operator() (unsigned int i) const   { return  (&x)[i]; }
  float&  operator[] (unsigned int i)         { return  (&x)[i]; }
  float   operator[] (unsigned int i) const   { return  (&x)[i]; }

  void normalize();

public:
  float   x, y, z;
};

Vector3 cross(const Vector3& p, const Vector3& q);