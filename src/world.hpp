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

#ifndef HEADER_CONSTRUO_WORLD_HPP
#define HEADER_CONSTRUO_WORLD_HPP

#include <vector>
#include "spring.hpp"
#include "particle.hpp"
#include "collider.hpp"
#include "bounding_box.hpp"

class Particle;
class ParticleFactory;
class Spring;

/** This class holds all particles and springs */
class World
{
private:
  static World* current_world;
public:
  /** @return pointer to the current world */
  static World* current() { return current_world; }

  friend class ParticleFactory;

public:
  typedef std::vector<Collider*> Colliders;
  typedef std::vector<Spring*>::iterator SpringIter;
  typedef std::vector<Spring*>::const_iterator CSpringIter;

public:
  /** Create an empty world */
  World ();

  /** Copy an existing world */
  World (const World& w);

  /** load a world from file */
  World (const std::string& filename);
  ~World ();

  void draw (ZoomGraphicContext& gc);
  void draw_springs (ZoomGraphicContext& gc);
  void draw_colliders (ZoomGraphicContext& gc);
  void draw_particles (ZoomGraphicContext& gc);

  void update (float delta);

  World* duplicate () { return new World (*this); }

  /** @return the particles closed to the given coordinates */
  Particle* get_particle (float x, float y);
  /** All particles inside the given rectangle */
  std::vector<Particle*> get_particles (float x1, float y1, float x2, float y2);
  Spring*   get_spring (float x, float y);

  void add_rect_collider(const Vector2d&, const Vector2d&);
  void add_spring (Particle*, Particle*);

  /** removes the given particle and all objects/springs which
      reference to it */
  void remove_particle (Particle*);

  /** remove the given spring */
  void remove_spring (Spring*);

  /** Remove the gives collider from the world */
  void remove_collider (Collider*);

  ParticleFactory* get_particle_mgr() { return m_particle_mgr; }
  std::vector<Spring*>& get_spring_mgr () { return m_springs; }
  Colliders& get_colliders() { return m_colliders; }

  /** removes everything from the world */
  void clear ();

  bool get_has_been_run () { return m_has_been_run; }

  /** Sets the velocity of all particles to zero, usefull if the
      particles are getting out of order (aka. explode). Also usefull
      to fix broken model files */
  void zero_out_velocity ();

  /** Write the current world down to a file */
  void write_lisp (const std::string& filename);

  /** @return the number of particles in the world */
  int get_num_particles();

  /** @return the number of springs in the world */
  int get_num_springs();

  /** Callculate the bounding box of the world from the particle and
   *  collider it contains. */
  BoundingBox calc_bounding_box();

private:
  void parse_scene(ReaderMapping const& reader);
  void parse_springs(ReaderCollection const& collection);
  void parse_particles(ReaderCollection const& collection);
  void parse_colliders(ReaderCollection const& collection);

private:
  /** Version number of the file, used to ensure backward compability */
  int m_file_version;
  bool m_has_been_run;
  ParticleFactory* m_particle_mgr;
  std::vector<Spring*> m_springs;
  Colliders m_colliders;

private:
  World& operator= (const World&);
};

#endif

/* EOF */
