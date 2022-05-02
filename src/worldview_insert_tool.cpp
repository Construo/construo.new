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

#include <float.h>
#include "colors.hpp"
#include "particle_factory.hpp"
#include "input_context.hpp"
#include "graphic_context.hpp"
#include "world.hpp"
#include "controller.hpp"
#include "worldview_component.hpp"
#include "worldview_insert_tool.hpp"
#include "world_gui_manager.hpp"

WorldViewInsertTool::WorldViewInsertTool() :
  m_current_particle(nullptr),
  m_particle_mass(0.1f)
{
}

WorldViewInsertTool::~WorldViewInsertTool()
{
}

void
WorldViewInsertTool::draw_background(ZoomGraphicContext& gc)
{
  float x = WorldViewComponent::instance()->zoom().screen_to_world_x(g_input_context->get_mouse_x());
  float y = WorldViewComponent::instance()->zoom().screen_to_world_y(g_input_context->get_mouse_y());

  World& world = *Controller::instance()->get_world();

  Particle* selected_particle = world.get_particle(x, y);
  if (selected_particle)
    {
      selected_particle->draw_highlight(gc);
    }
}

void
WorldViewInsertTool::draw_foreground(ZoomGraphicContext& gc)
{
  World& world = *Controller::instance()->get_world();

  Vector2d click_pos = WorldViewComponent::instance()->zoom().screen_to_world(g_input_context->get_mouse_pos());

  Vector2d new_particle_pos;

  float grid_size = WorldViewComponent::instance()->get_snap_size();

  if (WorldViewComponent::instance()->uses_grid())
    new_particle_pos = Vector2d(Math::round_to_float(click_pos.x, grid_size),
                                Math::round_to_float(click_pos.y, grid_size));
  else
    new_particle_pos = Vector2d(click_pos.x, click_pos.y);

  Spring* selected_spring = world.get_spring (click_pos.x, click_pos.y);

  if (selected_spring)
    {
      selected_spring->draw_highlight (gc);
    }

  if (m_current_particle)
    {
      gc.GraphicContext::draw_line (m_current_particle->pos, new_particle_pos,
                                     Colors::new_spring, 2);
    }

  float x = WorldViewComponent::instance()->zoom().screen_to_world_x(g_input_context->get_mouse_x ());
  float y = WorldViewComponent::instance()->zoom().screen_to_world_y(g_input_context->get_mouse_y ());

  Particle* selected_particle = world.get_particle (x, y);
  if (selected_particle)
    {
      selected_particle->draw_infos (gc);
    }
  else
    {
      gc.draw_fill_circle(new_particle_pos.x,
                           new_particle_pos.y,
                           3.0f,
                           Colors::highlight);
    }
}

void
WorldViewInsertTool::on_primary_button_press(float screen_x, float screen_y)
{
  World& world = *Controller::instance()->get_world();
  float x = WorldViewComponent::instance()->zoom().screen_to_world_x (screen_x);
  float y = WorldViewComponent::instance()->zoom().screen_to_world_y (screen_y);

  if (m_current_particle)
    {
      // We are going to place a second particle and connect the last an
      // the new one with a spring
      Particle* new_current_particle = world.get_particle (x, y);
      if (new_current_particle != m_current_particle)
        {
          if (new_current_particle) // connect to particles
            {
              world.add_spring (m_current_particle, new_current_particle);
            }
          else // add a new particle and connect it with the current one
            {
              Vector2d new_particle_pos;

              float grid_size = WorldViewComponent::instance()->get_snap_size();

              if (WorldViewComponent::instance()->uses_grid())
                new_particle_pos = Vector2d(Math::round_to_float(x, grid_size),
                                            Math::round_to_float(y, grid_size));
              else
                new_particle_pos = Vector2d(x, y);

              new_current_particle = world.get_particle_mgr()->add_particle(new_particle_pos,
                                                                            Vector2d(),
                                                                            m_particle_mass);
              world.add_spring (m_current_particle, new_current_particle);
            }
          // Lower the spring links count, since we have increased it
          // at insertion and now connected it to a real spring, so
          // its no longer needed
          m_current_particle->spring_links -= 1;

          m_current_particle = nullptr;
        }
      WorldGUIManager::instance()->ungrab_mouse(WorldViewComponent::instance());
    }
  else
    {
      // We are going to create a new particle and making it the
      // current one, so that the next click would result in a new
      // spring
      m_current_particle = world.get_particle (x, y);

      if (m_current_particle)
        {
          // We have clicked on a particle and made it  the current
        }
      else
        {
          Vector2d new_particle_pos;

          float grid_size = WorldViewComponent::instance()->get_snap_size();

          if (WorldViewComponent::instance()->uses_grid())
            new_particle_pos = Vector2d(Math::round_to_float(x, grid_size),
                                        Math::round_to_float(y, grid_size));
          else
            new_particle_pos = Vector2d(x, y);

          Particle* p = world.get_particle_mgr()->add_particle(new_particle_pos,
                                                               Vector2d(),
                                                               m_particle_mass);
          m_current_particle = p;
          // Increase the spring count so that the particle isn't cleaned up
          m_current_particle->spring_links += 1;
          WorldGUIManager::instance()->grab_mouse (WorldViewComponent::instance());
        }
    }
}

void
WorldViewInsertTool::on_primary_button_release (float x, float y)
{
}

void
WorldViewInsertTool::on_secondary_button_press (float screen_x, float screen_y)
{
  on_delete_press (screen_x, screen_y);
}

void
WorldViewInsertTool::on_secondary_button_release (float screen_x, float screen_y)
{
}

void
WorldViewInsertTool::on_delete_press (float screen_x, float screen_y)
{
  World& world = *Controller::instance()->get_world ();

  float x = WorldViewComponent::instance()->zoom().screen_to_world_x (screen_x);
  float y = WorldViewComponent::instance()->zoom().screen_to_world_y (screen_y);

  if (m_current_particle)
    { // We are currently creating a new spring, abort that
      m_current_particle->spring_links -= 1;
      m_current_particle = nullptr;
      WorldGUIManager::instance()->ungrab_mouse (WorldViewComponent::instance());
    }
  else
    {
      Spring*   spring   = world.get_spring (x, y);
      Particle* particle = world.get_particle (x, y);

      if (particle)
        {
          Controller::instance()->push_undo();
          world.remove_particle (particle);
        }
      else if (spring)
        {
          Controller::instance()->push_undo();
          world.remove_spring(spring);
        }
    }
}

void
WorldViewInsertTool::on_fix_press (float screen_x, float screen_y)
{
  float x = WorldViewComponent::instance()->zoom().screen_to_world_x (screen_x);
  float y = WorldViewComponent::instance()->zoom().screen_to_world_y (screen_y);

  Particle* particle = Controller::instance()->get_world ()->get_particle (x, y);
  if (particle)
    {
      particle->set_fixed (!particle->get_fixed ());
    }
}

/* EOF */
