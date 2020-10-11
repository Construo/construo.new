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
#include "construo_error.hpp"
#include "particle_factory.hpp"
#include "spring.hpp"

Spring::Spring (Particle* f, Particle* s, float l)
{
  particles.first  = f;
  particles.second = s;
  destroyed        = false;
  length           = l;

  stiffness   = 50.0f;
  damping     = .1f;
  max_stretch = 0.15f;

  f->spring_links += 1;
  s->spring_links += 1;
}

Spring::Spring (Particle* f, Particle* s)
{
  particles.first  = f;
  particles.second = s;
  destroyed        = false;
  length           = fabs((f->pos - s->pos).norm ());

  stiffness   = 50.0f;
  damping     = .1f;
  max_stretch = 0.15f;

  f->spring_links += 1;
  s->spring_links += 1;

  assert (length != 0);
}

Spring::Spring (World* world, ReaderMapping const& reader)
  : destroyed (false)
{
  int first_id = -1;
  int second_id = -1;
  length = -1;

  stiffness   = 50.0f;
  damping     = .1f;
  max_stretch = 0.15f;

  reader.read("first", first_id);
  reader.read("second", second_id);
  reader.read("length", length);
  reader.read("stiffness", stiffness);
  reader.read("damping", damping);
  reader.read("maxstretch", max_stretch);

  particles.first  = world->get_particle_mgr()->lookup_particle (first_id);
  particles.second = world->get_particle_mgr()->lookup_particle (second_id);

  if (particles.first == 0 || particles.second == 0)
    {
      throw ConstruoError ("Spring: Pair lookup failed");
    }

  particles.first->spring_links  += 1;
  particles.second->spring_links += 1;

  if (length == -1)
    {
      //std::cout << "Spring: length missing in data file, recalculating" << std::endl;
      length = fabs((particles.first->pos - particles.second->pos).norm ());
    }
}

Spring::~Spring ()
{
  particles.first->spring_links -= 1;
  particles.second->spring_links -= 1;
}

void
Spring::update (float delta)
{
  Vector2d dist = particles.first->pos - particles.second->pos;

  // Calculate the stretchness of the spring, 0.0 if unstretch, else
  // <> 0
  float stretch = (dist.norm () - length);

  //std::cout << "Stretch: " << stretch << std::endl;
  if (fabs(stretch/length) > max_stretch &&
      length > 10.0f) // atomar spring
    { // If the spring is streched above limits, let it get destroyed
      destroyed = true;
    }
  else
    {
      stretch *= stiffness;
      float dterm = (dist.dot(particles.first->velocity - particles.second->velocity) * damping)/dist.norm ();

      dist.normalize ();
      Vector2d force = dist * (stretch + dterm);

      /*std::cout << "DTerm: " << dterm << " HTerm: " << stretch
                << " Force: " << force
                << std::endl;*/

      particles.first->add_force (-force);
      particles.second->add_force (force);
    }
}

void
Spring::draw (ZoomGraphicContext* gc)
{
  Vector2d dist = particles.first->pos - particles.second->pos;
  float stretch = fabs(dist.norm ()/length - 1.0f) * 10.0f;

  float color = fabs((stretch/max_stretch));

  if (particles.first->pos.y  < 598.5f
      ||
      particles.second->pos.y < 598.5f)
    {
      gc->GraphicContext::draw_line(particles.first->pos,
                                    particles.second->pos,
                                    Color(color, 1.0f - color, 0.0f),
                                    2);
    }
}

void
Spring::draw_highlight (ZoomGraphicContext* gc)
{
  gc->GraphicContext::draw_line (particles.first->pos, particles.second->pos,
                                 Colors::highlight, 4);
}

void
Spring::serialize(LispWriter& writer)
{
  writer.begin_object("spring")
    .write("first", particles.first->get_id())
    .write("second", particles.second->get_id())
    .write ("length", length)
    .write("stiffness", stiffness)
    .write("damping", damping)
    .write("maxstretch", max_stretch)
    .end_object();
}

void
Spring::recalc_length ()
{
  length = fabs((particles.first->pos - particles.second->pos).norm ());
}

/* EOF */
