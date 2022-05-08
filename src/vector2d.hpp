// Construo - A wire-frame construction gamee
// Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_VECTOR2D_HPP
#define HEADER_VECTOR2D_HPP

#include <math.h>
#include <iostream>

class Vector2d
{
public:
  float x;
  float y;

  //  Vector2d ()
  //  : x(0), y(0)
  //{}

  Vector2d (float x_, float y_)
    : x (x_), y (y_)
  {}

  inline
  void operator+= (const Vector2d& vec) {
    x += vec.x;
    y += vec.y;
  }

  inline
  void operator-= (const Vector2d& vec) {
    x -= vec.x;
    y -= vec.y;
  }

  inline
  void operator*= (float f) {
    x *= f;
    y *= f;
  }

  inline
  Vector2d operator+ (const Vector2d& vec) const  {
    return Vector2d(x + vec.x, y + vec.y);
  }

  inline
  float dot(const Vector2d& vec) const {
    return (x * vec.x) + (y * vec.y);
  }

  inline
  Vector2d operator- () const  {
    return Vector2d(-x, -y);
  }

  inline
  Vector2d operator- (const Vector2d& vec) const  {
    return Vector2d(x - vec.x, y - vec.y);
  }

  inline
  Vector2d operator* (float f) const  {
    return Vector2d(x * f, y * f);
  }

  /** @return the length of the vector, also known as norm */
  inline
  float norm() const {
    return sqrt (x*x + y*y);
  }


  inline
  void normalize() {
    float f = norm();
    if (f!=0)
      {
        x /= f;
        y /= f;
      }
  }

  static
  float distance(const Vector2d& a, const Vector2d& b) {
    return (a - b).norm();
  }
};

inline
std::ostream& operator << (std::ostream& os, const Vector2d& v)
{
  return os << "[" << v.x << ", " << v.y << "]";
}

#endif

/* EOF */
