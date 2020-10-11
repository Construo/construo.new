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

#include "colors.hpp"
#include "math.hpp"
#include "lisp_reader.hpp"
#include "string_utils.hpp"
#include "particle.hpp"

using namespace StringUtils;

Particle::Particle (int i, const Vector2d& arg_pos, const Vector2d& arg_velocity, float m, bool f)
  : id (i),
    pos (arg_pos),
    velocity (arg_velocity),
    mass (m),
    fixed (f),
    totale_force (0,0),
    spring_links (0)
{
}

Particle::Particle (const Particle& p)
  : id (p.id),
    pos (p.pos),
    velocity (p.velocity),
    mass (p.mass),
    fixed (p.fixed),
    totale_force (0,0),
    spring_links (0)
{
}

void
Particle::serialize(LispWriter& writer)
{
  writer.begin_object("particle")
    .write("id", id)
    .write("pos", pos)
    .write("velocity", velocity)
    .write("fixed", fixed)
    .write("mass", mass)
    .end_object();
}

void
Particle::draw_highlight (ZoomGraphicContext* gc)
{
  gc->get_parent_gc()->draw_fill_circle (gc->world_to_screen(pos),
                                         Math::round(Math::max(6.0f, get_mass() + 3)),
                                         Colors::highlight);
}

void
Particle::draw_infos (ZoomGraphicContext* gc)
{
  Vector2d p = gc->world_to_screen(pos);
  draw_velocity_vector (gc);
  gc->get_parent_gc()->draw_string (p + Vector2d(20.0f, 5.0f),
                                    "Particle: " + to_string (pos));
  gc->get_parent_gc()->draw_string (p + Vector2d(20.0f, 25.0f),
                                    "Fixed:    " + to_string (fixed));
  gc->get_parent_gc()->draw_string (p + Vector2d(20.0f, 45.0f),
                                    "Mass :    " + to_string (get_mass()));
  gc->get_parent_gc()->draw_string (p + Vector2d(20.0f, 70.0f),
                                    "Links :    " + to_string (spring_links));
}

void
Particle::draw (ZoomGraphicContext* gc)
{
  if (pos.y < 598.5f)
    {
      if (fixed)
        {
          gc->get_parent_gc()->draw_fill_circle (gc->world_to_screen(pos),
                                                 4,
                                                 Color(0.6f, 0.6f, 0.6f));
        }
      else
        {
          gc->get_parent_gc()->draw_fill_circle (gc->world_to_screen(pos),
                                                 Math::round(Math::max(3.0f, get_mass())),
                                                 Color(1.0f, 0.0f, 0.0f));
        }
    }
}

void
Particle::draw_velocity_vector (ZoomGraphicContext* gc)
{
  gc->draw_line (int (pos.x), int (pos.y),
                 int (pos.x + velocity.x), int (pos.y + velocity.y),
                 Color (0.0f, 0.0f, 1.0f));
}

void
Particle::update (float delta)
{
  const float max_velocity = 1000.0f;

  if (fixed) return;

  velocity += totale_force * delta * (1.0f/mass);

  // damping
  if (0)
    velocity -= (velocity * (1.0f/mass) * delta) * 0.001f;

  //velocity *= .999999f ;

  pos += velocity * delta;

  float collision_damp = 0.2;

#if 0 // FIXME: Replace this with a generic shape collision handling thing
  // Calc collision with screen x border
  if (pos.x < 0) {
    velocity.x =  fabs(velocity.x);
    pos.x = 0;
    velocity *= collision_damp;
  } else if (pos.x > 799) {
    velocity.x =  -fabs(velocity.x);
    pos.x = 799;
    velocity *= collision_damp;
  }

  // Calc collision with screen y border
  if (pos.y < 0) {
    velocity.y =  fabs(velocity.y);
    pos.y = 0;
    velocity *= collision_damp;
  } else
#endif

    if (pos.y > 599) {
      velocity.y =  -fabs(velocity.y);
      pos.y = 599;
      velocity *= collision_damp;
    }

  /*
    Vector2d dist = pos - Vector2d (400, 300);
    if (dist.norm () < 50.0f)
    {
    velocity = -velocity;
    }*/
  clear_force ();

  // Avoid to fast things
  if (velocity.norm () > max_velocity)
    {
      velocity.normalize();
      velocity *= max_velocity;
    }
}


/* EOF */
