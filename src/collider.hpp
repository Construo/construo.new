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

#ifndef HEADER_CONSTRUO_COLLIDER_HPP
#define HEADER_CONSTRUO_COLLIDER_HPP

#include <memory>

#include <glm/glm.hpp>

#include "particle.hpp"
#include "bounding_box.hpp"
#include "graphic_context.hpp"

class Collider
{
private:

public:
  virtual ~Collider() {}

  virtual std::unique_ptr<Collider> duplicate() const =0;

  virtual glm::vec2 get_pos() =0;
  virtual void set_pos(const glm::vec2&) =0;

  /** @return true if the collider is at the given world coordinates,
      used for drag&drop */
  virtual bool is_at (const glm::vec2& pos) =0;

  /** act on a particle */
  virtual void bounce () =0;

  virtual BoundingBox get_bounding_box() const =0;
};

#endif

/* EOF */
