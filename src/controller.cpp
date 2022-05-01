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

#include "worldview_component.hpp"
#include "construo_error.hpp"
#include "controller.hpp"

Controller* Controller::instance_ = nullptr;

Controller::Controller ()
{
  instance_  = this;
  running    = false;
  slow_down  = false;
  action_cam = false;
  hide_dots  = false;
  world      = new World ();
}

Controller::Controller (const std::string& filename)
{
  instance_  = this;
  running    = false;
  slow_down  = false;
  action_cam = false;
  hide_dots  = false;
  world     = new World (filename);
}

Controller::~Controller ()
{
  instance_ = nullptr;
}

void
Controller::load_world (const std::string& filename)
{
  if (world)
    undo_world_stack.push_back(world);

  //std::cout << "Loading World..." << std::endl;
  world = new World (filename);

  WorldViewComponent::instance()->on_world_change();

  running = false;
  //std::cout << "Loading World... DONE" << std::endl;
}

void
Controller::save_world (const std::string& filename)
{
  //std::cout << "Saving World..." << std::endl;
  world->write_lisp (filename);
  //std::cout << "Saving World... DONE" << std::endl;
}


std::string
Controller::get_slot_filename(int n)
{
  return "/user/" + std::string("quicksave") + char('0' + n) + ".construo";
}

void
Controller::save_to_slot (int n)
{
  try {
    save_world (get_slot_filename (n));
  } catch (std::exception const& err) {
    print_exception(err);
  }
}

void
Controller::load_from_slot (int n)
{
  try {
    load_world (get_slot_filename (n));
  } catch (std::exception const& err) {
    print_exception(err);
  }
}

void
Controller::update ()
{
  float delta = delta_manager.getset ();

  if (running)
    {
      float min_skip;

      if (slow_down)
        {
          delta /= 50.0f/20.0f;
          min_skip = 0.0007f;
        }
      else
        {
          delta /= 5.0f/20.0f;
          min_skip = 0.02; // 0.02
        }

      float i = 0.0f;
      while (i < delta)
        {
          world->update (min_skip);
          i += min_skip;
        }
    }
}

void
Controller::start_simulation ()
{
  if (!running)
    undo_world_stack.push_back(world->duplicate());

  if (undo_world_stack.size() > 100)
    {
      // FIXME: shrink stack here
      //delete *undo_world_stack.front();
      //std::cout << "Stak
    }

  running = !running;
}

void
Controller::push_undo()
{
  undo_world_stack.push_back(world->duplicate());
}

void
Controller::clear_world ()
{
  std::cout << "Controller: Clear" << std::endl;
  undo_world_stack.push_back(world);
  world = new World ();
  running = false;
}

void
Controller::undo ()
{
#ifdef DEBUG
  std::cout << "Controller::undo (): undostack: " << undo_world_stack.size()
            << " redostack: " << redo_world_stack.size() << std::endl;
#endif

  if (!undo_world_stack.empty())
    {
      //delete world; // fixme: memory hole
      redo_world_stack.push_back (world);
      world = undo_world_stack.back();
      undo_world_stack.pop_back();
      running = false;
    }
  else
    {
      std::cout << "Undo stack empty" << std::endl;
    }
}

void
Controller::redo ()
{
  if (!redo_world_stack.empty())
    {
      undo_world_stack.push_back (world);
      world = redo_world_stack.back();
      redo_world_stack.pop_back();
      running = false;
    }
  else
    {
      std::cout << "Redo stack empty" << std::endl;
    }
}

void
Controller::set_action_cam(bool a)
{
  action_cam = a;
}

bool
Controller::get_action_cam()
{
  return action_cam;
}

void
Controller::set_hide_dots (bool d)
{
  hide_dots = d;
}

bool
Controller::get_hide_dots ()
{
  return hide_dots;
}

/* EOF */
