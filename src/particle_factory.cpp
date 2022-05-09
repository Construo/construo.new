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

#include <float.h>
#include <algorithm>
#include "zoom_graphic_context.hpp"
#include "particle.hpp"
#include "construo_error.hpp"
#include "world.hpp"
#include "particle_factory.hpp"

ParticleFactory::ParticleFactory() :
  m_particles(),
  m_particle_id_count(0)
{
}

ParticleFactory::ParticleFactory(const ParticleFactory& pmgr) :
  m_particles(),
  m_particle_id_count(pmgr.m_particle_id_count)
{
  for (auto i = pmgr.m_particles.begin (); i != pmgr.m_particles.end (); ++i)
    m_particles.push_back(new Particle(**i));
}

ParticleFactory&
ParticleFactory::operator= (const ParticleFactory& pmgr)
{
  ConstruoAssert (0, "Don't use this");
  for (auto i = pmgr.m_particles.begin ();
       i != pmgr.m_particles.end ();
       ++i)
    {
      m_particles.push_back (new Particle (*(*i)));
    }
  return *this;
}

Particle*
ParticleFactory::add_particle(int id, const glm::vec2& pos, const glm::vec2& velocity,
                       float mass, bool fixed)
{
  if (id >= m_particle_id_count) {
    m_particle_id_count = id + 1;
  }

  Particle* p = new Particle(id, pos, velocity, mass, fixed);
  m_particles.push_back(p);
  return p;
}

Particle*
ParticleFactory::add_particle (const glm::vec2& pos, const glm::vec2& velocity, float mass, bool fixed)
{
  return add_particle(m_particle_id_count++, pos, velocity, mass, fixed);
}

Particle*
ParticleFactory::add_particle (const Particle& particle)
{
  Particle* p = new Particle (particle);
  p->id = m_particle_id_count++,
  m_particles.push_back(p);
  return p;
}

void
ParticleFactory::remove_particle (Particle* p)
{
  // Remove the particle itself
  for (auto i = m_particles.begin (); i != m_particles.end (); ++i)
    {
      if (*i == p)
        {
          delete *i;
          m_particles.erase(i);
          return;
        }
    }
}

struct particle_obsolete
{
  inline bool operator()(Particle* p)
  {
    return (p->spring_links == 0
            && p->velocity.x == 0
            && std::fabs(p->velocity.y) < 0.1f);
  }
};

void
ParticleFactory::update (float delta)
{
  for (auto i = m_particles.begin (); i != m_particles.end (); ++i)
    (*i)->update(delta);

  // FIXME: There is no need to do this on any update, doing it only
  //once a second should be enough
  m_particles.erase(std::remove_if(m_particles.begin(), m_particles.end(),
                                   particle_obsolete()),
                    m_particles.end());
}

Particle*
ParticleFactory::lookup_particle (int id)
{
  // FIXME: Could need optimization
  for (auto i = m_particles.begin(); i != m_particles.end(); ++i)
    {
      if ((*i)->get_id () == id)
        return *i;
    }
  return nullptr;
}

void
ParticleFactory::clear ()
{
  for (auto i = m_particles.begin (); i != m_particles.end (); ++i)
    delete *i;
  m_particles.clear ();
}

void
ParticleFactory::write_lisp(prio::Writer& writer)
{
  writer.begin_collection("particles");
  for (auto i = m_particles.begin (); i != m_particles.end (); ++i) {
    (*i)->serialize(writer);
  }
  writer.end_collection();
}

/* EOF */
