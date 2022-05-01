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
#include "controller.hpp"
#include "root_graphic_context.hpp"
#include "world_gui_manager.hpp"
#include "worldview_tool.hpp"
#include "worldview_insert_tool.hpp"
#include "worldview_select_tool.hpp"
#include "worldview_zoom_tool.hpp"
#include "worldview_collider_tool.hpp"
#include "worldview_component.hpp"

WorldViewComponent* WorldViewComponent::instance_;

WorldViewComponent::WorldViewComponent () :
  GUIComponent()
{
  instance_ = this;

  scrolling = false;
  use_grid  = false;
  grid_base_size = 10;
  grid_constant = 5;
  grid_scale_factor = 1;
  grid_snap_factor = 2;

  select_tool   = new WorldViewSelectTool ();
  insert_tool   = new WorldViewInsertTool ();
  zoom_tool     = new WorldViewZoomTool ();
  collider_tool = new WorldViewColliderTool ();

  current_tool = insert_tool;
  mode = INSERT_MODE;

  on_world_change();
}

void
WorldViewComponent::set_mode (Mode m)
{
  current_tool->deactivate ();

  if (m == INSERT_MODE)
    {
      current_tool = insert_tool;
      mode = INSERT_MODE;
      graphic_context->set_cursor(CURSOR_INSERT);
    }
  else if (m == SELECT_MODE)
    {
      current_tool = select_tool;
      mode = SELECT_MODE;
      graphic_context->set_cursor(CURSOR_SELECT);
    }
  else if (m == ZOOM_MODE)
    {
      current_tool = zoom_tool;
      mode = ZOOM_MODE;
      graphic_context->set_cursor(CURSOR_ZOOM);
    }
  else if (m == COLLIDER_MODE)
    {
      current_tool = collider_tool;
      mode = COLLIDER_MODE;
      graphic_context->set_cursor(CURSOR_COLLIDER);
    }
  else
    {
      std::cout << "Unknown Mode" << std::endl;
      assert (false);
    }

  current_tool->activate ();
}

WorldViewComponent::~WorldViewComponent ()
{
  instance_ = 0;
}

float
WorldViewComponent::get_grid_size()
{
	return grid_base_size / pow(grid_constant, Math::get_exp_n(gc.get_zoom() * grid_scale_factor, grid_constant));
}

float
WorldViewComponent::get_snap_size()
{
  return get_grid_size() / grid_snap_factor;
}

void
WorldViewComponent::draw_grid()
{
  Color color = Colors::grid_color;
  Color color2 = Colors::grid_color2;

  float grid_size = get_grid_size();

  float start_x = Math::round_to_float(gc.screen_to_world_x(0), grid_size) - grid_size;
  float end_x   = Math::round_to_float(gc.screen_to_world_x(gc.get_width()), grid_size) + grid_size;

  float start_y = Math::round_to_float(gc.screen_to_world_y(0), grid_size) - grid_size;
  float end_y   = Math::round_to_float(gc.screen_to_world_y(gc.get_height()), grid_size) + grid_size;

  gc.push_quick_draw();
  for(float y = start_y; y < end_y; y += grid_size)
    gc.draw_line(start_x, y,
                 end_x, y,
                 ((int(y / grid_size) % grid_constant) == 0) ? color2 : color, 1);

  for(float x = start_x; x < end_x; x += grid_size)
    gc.draw_line(x, start_y,
                 x, end_y,
                 ((int(x / grid_size) % grid_constant) == 0) ? color2 : color, 1);
  gc.pop_quick_draw();
}

void
WorldViewComponent::draw_ground()
{
  GraphicContext* parent_gc = gc.get_parent_gc();

  if (gc.screen_to_world_y(parent_gc->get_height()) >= 599)
    {
      gc.draw_fill_rect(gc.screen_to_world_x(0),
                        599,
                        gc.screen_to_world_x(parent_gc->get_width()),
                        gc.screen_to_world_y(parent_gc->get_height()),
                        Colors::ground_color);

      Color color = Colors::ground_grid_color;

      int step_size = 100;

      int start_x = Math::round_to(gc.screen_to_world_x(0), step_size) - step_size;
      int end_x   = Math::round_to(gc.screen_to_world_x(gc.get_width()), step_size) + step_size;

      int start_y = 599;
      int end_y   = Math::round_to(gc.screen_to_world_y(gc.get_height()), step_size) + step_size;

      gc.push_quick_draw();
      for(int y = start_y; y < end_y; y += step_size)
        gc.draw_line(start_x, y,
                     end_x, y,
                     color, 1);

      for(int x = start_x; x < end_x; x += step_size)
        gc.draw_line(x, start_y,
                     x, end_y,
                     color, 1);
      gc.pop_quick_draw();

      gc.draw_rect(gc.screen_to_world_x(0),
                   599,
                   gc.screen_to_world_x(parent_gc->get_width()),
                   gc.screen_to_world_y(parent_gc->get_height()),
                   Colors::rect_collider_bg);
    }
}

void
WorldViewComponent::draw (GraphicContext* parent_gc)
{
  //int x = gc.screen_to_world_x (input_context->get_mouse_x ());
  //int y = gc.screen_to_world_y (input_context->get_mouse_y ());

  gc.set_parent_gc (parent_gc);

  if (use_grid)
    draw_grid();

  draw_ground();

  World& world = *Controller::instance()->get_world();

  if (Controller::instance()->get_action_cam()
      && Controller::instance()->is_running())
    {
      // Live Action Cam
      const BoundingBox& box = world.calc_bounding_box();
      // Zoom to the bounding box
      gc.zoom_to((int) box.x1, (int)box.y1,
                 (int)box.x2, (int)box.y2);
      // Zoom out two times so that the area isn't covered up by the
      // GUI
      gc.zoom_out (get_width()/2, get_height()/2);
      gc.zoom_out (get_width()/2, get_height()/2);
    }

  current_tool->draw_background (&gc);

  if (0) // draw bounding box
    {
      const BoundingBox& box = world.calc_bounding_box();
      gc.draw_rect(box.x1, box.y1, box.x2, box.y2,
                   Color(1.0f, 1.0f, 1.0f));
    }

  world.draw_colliders (&gc);
  world.draw_springs (&gc);
  if (!Controller::instance()->get_hide_dots())
    world.draw_particles (&gc);

  current_tool->draw_foreground (&gc);

  if (0)
    {
      switch (mode)
        {
        case ZOOM_MODE:
          parent_gc->draw_string (10, parent_gc->get_height () - 15, "[  Zoom Mode  ]");
          break;
        case INSERT_MODE:
          parent_gc->draw_string (10, parent_gc->get_height () - 15, "[ Insert Mode ]");
          break;
        case SELECT_MODE:
          parent_gc->draw_string (10, parent_gc->get_height () - 15, "[ Select Mode ]");
          break;
        case COLLIDER_MODE:
          parent_gc->draw_string (10, parent_gc->get_height () - 15, "[Collider Mode]");
          break;
        }
    }

  //const WorldBoundingBox& box = world.calc_bounding_box();
  //gc.flip (int(box.x1), int(box.y1), int(box.x2), int(box.y2));
}

void
WorldViewComponent::wheel_up (int x, int y)
{
  gc.zoom_in (x, y);
}

void
WorldViewComponent::wheel_down (int x, int y)
{
  gc.zoom_out (x, y);
}

void
WorldViewComponent::on_button_press (int button_id, int x, int y)
{
  current_tool->on_button_press (button_id, x, y);
}

void
WorldViewComponent::on_primary_button_press (int screen_x, int screen_y)
{
  current_tool->on_primary_button_press (screen_x, screen_y);
}

void
WorldViewComponent::on_primary_button_release (int screen_x, int screen_y)
{
  current_tool->on_primary_button_release (screen_x, screen_y);
}

void
WorldViewComponent::on_secondary_button_press (int screen_x, int screen_y)
{
  current_tool->on_secondary_button_press (screen_x, screen_y);
}

void
WorldViewComponent::on_secondary_button_release (int screen_x, int screen_y)
{
  current_tool->on_secondary_button_release (screen_x, screen_y);
}

void
WorldViewComponent::on_delete_press (int screen_x, int screen_y)
{
  current_tool->on_delete_press (screen_x, screen_y);
}

void
WorldViewComponent::on_duplicate_press (int screen_x, int screen_y)
{
  current_tool->on_duplicate_press (screen_x, screen_y);
}

void
WorldViewComponent::on_join_press (int x, int y)
{
  current_tool->on_join_press (x, y);
}

void
WorldViewComponent::on_fix_press (int screen_x, int screen_y)
{
  current_tool->on_fix_press (screen_x, screen_y);
}

void
WorldViewComponent::scroll_left ()
{
  gc.translate_offset (-20, 0);
}

void
WorldViewComponent::scroll_right ()
{
  gc.translate_offset (20, 0);
}

void
WorldViewComponent::scroll_up ()
{
  gc.translate_offset (0, -20);
}

void
WorldViewComponent::scroll_down ()
{
  gc.translate_offset (0, 20);
}

void
WorldViewComponent::on_tertiary_button_press (int x, int y)
{
  scrolling = true;
  graphic_context->push_cursor();
  graphic_context->set_cursor(CURSOR_SCROLL);

  x_offset = gc.get_x_offset ();
  y_offset = gc.get_y_offset ();
  WorldGUIManager::instance()->grab_mouse (this);

  scroll_pos_x = gc.screen_to_world_x(x);
  scroll_pos_y = gc.screen_to_world_y(y);
}

void
WorldViewComponent::on_tertiary_button_release (int x, int y)
{
  graphic_context->pop_cursor();
  scrolling = false;
  WorldGUIManager::instance()->ungrab_mouse (this);
}

void
WorldViewComponent::on_mouse_move (int x, int y, int of_x, int of_y)
{
  if (scrolling)
    {
      int new_scroll_pos_x = int(x/gc.get_zoom() - x_offset);
      int new_scroll_pos_y = int(y/gc.get_zoom() - y_offset);

      gc.set_offset (x_offset + (new_scroll_pos_x - scroll_pos_x),
                     y_offset + (new_scroll_pos_y - scroll_pos_y));

    }
  else
    {
      current_tool->on_mouse_move (x, y, of_x, of_y);
    }
}

void
WorldViewComponent::on_scale_press(int x, int y)
{
  current_tool->on_scale_press(x,y);
}

void
WorldViewComponent::on_grid_press(int x, int y)
{
  use_grid = !use_grid;
}

float
WorldViewComponent::get_zoom ()
{
  return gc.get_zoom ();
}

void
WorldViewComponent::on_world_change()
{
  World& world = *Controller::instance()->get_world();

  const BoundingBox& box = world.calc_bounding_box();
  // Zoom to the bounding box
  gc.zoom_to((int) box.x1, (int)box.y1,
             (int)box.x2, (int)box.y2);
  // Zoom out two times so that the area isn't covered up by the
  // GUI
  gc.zoom_out (get_width()/2, get_height()/2);
  gc.zoom_out (get_width()/2, get_height()/2);
}

/* EOF */
