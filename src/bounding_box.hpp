// Construo - A wire-frame construction game
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

#ifndef HEADER_BOUNDING_BOX_HPP
#define HEADER_BOUNDING_BOX_HPP

#include <glm/fwd.hpp>

class BoundingBox
{
public:
  float x1;
  float y1;
  float x2;
  float y2;

  BoundingBox();
  BoundingBox(float x1_, float y1_, float x2_, float y2_);

  /** Join the current bounding box with another bounding box, letting
   *  the resulting box cover both boxes */
  void join(const BoundingBox& box);

  /** Let the bounding box be enlarge so that it also coveres pos */
  void join(const Vector2d& pos);
};

std::ostream& operator << (std::ostream& os, const BoundingBox& box);

#endif

/* EOF */
