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

WorldViewComponent::WorldViewComponent() :
  GUIComponent(),
  m_zoom(0, 0, g_graphic_context->get_width(), g_graphic_context->get_height()),
  m_use_grid(false),
  m_grid_base_size(10),
  m_grid_constant(5),
  m_grid_scale_factor(1),
  m_grid_snap_factor(2),
  m_scrolling(false),
  m_scroll_pos_x(0.0f),
  m_scroll_pos_y(0.0f),
  m_x_offset(0.0f),
  m_y_offset(0.0f),
  m_select_tool(std::make_unique<WorldViewSelectTool>()),
  m_insert_tool(std::make_unique<WorldViewInsertTool>()),
  m_zoom_tool(std::make_unique<WorldViewZoomTool>()),
  m_collider_tool(std::make_unique<WorldViewColliderTool>()),
  m_current_tool(m_insert_tool.get()),
  m_mode(INSERT_MODE)
{
  instance_ = this;

  on_world_change();
}

void
WorldViewComponent::set_mode(Mode m)
{
  m_current_tool->deactivate();

  if (m == INSERT_MODE)
  {
    m_current_tool = m_insert_tool.get();
    m_mode = INSERT_MODE;
    g_graphic_context->set_cursor(CURSOR_INSERT);
  }
  else if (m == SELECT_MODE)
  {
    m_current_tool = m_select_tool.get();
    m_mode = SELECT_MODE;
    g_graphic_context->set_cursor(CURSOR_SELECT);
  }
  else if (m == ZOOM_MODE)
  {
    m_current_tool = m_zoom_tool.get();
    m_mode = ZOOM_MODE;
    g_graphic_context->set_cursor(CURSOR_ZOOM);
  }
  else if (m == COLLIDER_MODE)
  {
    m_current_tool = m_collider_tool.get();
    m_mode = COLLIDER_MODE;
    g_graphic_context->set_cursor(CURSOR_COLLIDER);
  }
  else
  {
    std::cout << "Unknown Mode" << std::endl;
    assert(false);
  }

  m_current_tool->activate ();
}

WorldViewComponent::~WorldViewComponent()
{
  instance_ = nullptr;
}

float
WorldViewComponent::get_grid_size()
{
  return m_grid_base_size / powf(static_cast<float>(m_grid_constant),
                                 static_cast<float>(Math::get_exp_n(m_zoom.get_zoom() * m_grid_scale_factor, m_grid_constant)));
}

float
WorldViewComponent::get_snap_size()
{
  return get_grid_size() / m_grid_snap_factor;
}

void
WorldViewComponent::draw_grid(ZoomGraphicContext& gc)
{
  Color color = Colors::grid_color;
  Color color2 = Colors::grid_color2;

  float grid_size = get_grid_size();

  float start_x = Math::round_to_float(m_zoom.screen_to_world_x(0), grid_size) - grid_size;
  float end_x   = Math::round_to_float(m_zoom.screen_to_world_x(m_zoom.bounding_width()), grid_size) + grid_size;

  float start_y = Math::round_to_float(m_zoom.screen_to_world_y(0), grid_size) - grid_size;
  float end_y   = Math::round_to_float(m_zoom.screen_to_world_y(m_zoom.bounding_height()), grid_size) + grid_size;

  gc.push_quick_draw();
  for(float y = start_y; y < end_y; y += grid_size) {
    gc.draw_line(start_x, y,
                 end_x, y,
                 ((int(y / grid_size) % m_grid_constant) == 0) ? color2 : color, 1);
  }

  for(float x = start_x; x < end_x; x += grid_size) {
    gc.draw_line(x, start_y,
                 x, end_y,
                 ((int(x / grid_size) % m_grid_constant) == 0) ? color2 : color, 1);
  }

  gc.pop_quick_draw();
}

void
WorldViewComponent::draw_ground(ZoomGraphicContext& gc)
{
  GraphicContext& parent_gc = gc.get_parent_gc();

  if (gc.zoom().screen_to_world_y(parent_gc.get_height()) >= 599)
  {
    gc.draw_fill_rect(gc.zoom().screen_to_world_x(0),
                      599,
                      gc.zoom().screen_to_world_x(parent_gc.get_width()),
                      gc.zoom().screen_to_world_y(parent_gc.get_height()),
                      Colors::ground_color);

    // draw grid
    {
      gc.push_quick_draw();

      float const step_size = 100.0f;

      float const start_x = Math::round_to_float(gc.zoom().screen_to_world_x(gc.zoom().bounding_x1()), step_size) - step_size;
      float const end_x   = Math::round_to_float(gc.zoom().screen_to_world_x(gc.zoom().bounding_x2()), step_size) + step_size;

      float const start_y = 599;
      float const end_y   = Math::round_to_float(gc.zoom().screen_to_world_y(gc.zoom().bounding_y2()), step_size) + step_size;

      for(float y = start_y; y < end_y; y += step_size) {
        gc.draw_line(start_x, y,
                     end_x, y,
                     Colors::ground_grid_color, 1);
      }

      for(float x = start_x; x < end_x; x += step_size) {
        gc.draw_line(x, start_y,
                     x, end_y,
                     Colors::ground_grid_color, 1);
      }
      gc.pop_quick_draw();
    }

    gc.draw_rect(gc.zoom().screen_to_world_x(0),
                 599,
                 gc.zoom().screen_to_world_x(parent_gc.get_width()),
                 gc.zoom().screen_to_world_y(parent_gc.get_height()),
                 Colors::rect_collider_bg);
  }
}

void
WorldViewComponent::draw(GraphicContext& parent_gc)
{
  m_zoom.set_bounding_box(m_x, m_y, m_width, m_height);
  ZoomGraphicContext gc(parent_gc, m_zoom);

  if (m_use_grid) {
    draw_grid(gc);
  }

  draw_ground(gc);

  World& world = *Controller::instance()->get_world();

  if (Controller::instance()->get_action_cam()
      && Controller::instance()->is_running())
  {
    // Live Action Cam
    const BoundingBox& box = world.calc_bounding_box();
    // Zoom to the bounding box
    m_zoom.zoom_to(static_cast<int>(box.x1), static_cast<int>(box.y1),
                 static_cast<int>(box.x2), static_cast<int>(box.y2));
    // Zoom out two times so that the area isn't covered up by the
    // GUI
    m_zoom.zoom_out(get_width()/2, get_height()/2);
    m_zoom.zoom_out(get_width()/2, get_height()/2);
  }

  m_current_tool->draw_background(gc);

  if (0) // draw bounding box
  {
    const BoundingBox& box = world.calc_bounding_box();
    gc.draw_rect(box.x1, box.y1, box.x2, box.y2,
                 Color(1.0f, 1.0f, 1.0f));
  }

  world.draw_colliders(gc);
  world.draw_springs(gc);
  if (!Controller::instance()->get_hide_dots()) {
    world.draw_particles(gc);
  }

  m_current_tool->draw_foreground(gc);

  if (0)
  {
    switch (m_mode)
    {
      case ZOOM_MODE:
        parent_gc.draw_string(10, parent_gc.get_height() - 15, "[  Zoom Mode  ]");
        break;
      case INSERT_MODE:
        parent_gc.draw_string(10, parent_gc.get_height() - 15, "[ Insert Mode ]");
        break;
      case SELECT_MODE:
        parent_gc.draw_string(10, parent_gc.get_height() - 15, "[ Select Mode ]");
        break;
      case COLLIDER_MODE:
        parent_gc.draw_string(10, parent_gc.get_height() - 15, "[Collider Mode]");
        break;
    }
  }

  //const WorldBoundingBox& box = world.calc_bounding_box();
  //gc.flip(int(box.x1), int(box.y1), int(box.x2), int(box.y2));
}

void
WorldViewComponent::wheel_up(float x, float y)
{
  m_zoom.zoom_in(x, y);
}

void
WorldViewComponent::wheel_down(float x, float y)
{
  m_zoom.zoom_out(x, y);
}

void
WorldViewComponent::on_button_press(int button_id, float x, float y)
{
  m_current_tool->on_button_press(button_id, x, y);
}

void
WorldViewComponent::on_primary_button_press(float screen_x, float screen_y)
{
  m_current_tool->on_primary_button_press(screen_x, screen_y);
}

void
WorldViewComponent::on_primary_button_release(float screen_x, float screen_y)
{
  m_current_tool->on_primary_button_release(screen_x, screen_y);
}

void
WorldViewComponent::on_secondary_button_press(float screen_x, float screen_y)
{
  m_current_tool->on_secondary_button_press(screen_x, screen_y);
}

void
WorldViewComponent::on_secondary_button_release(float screen_x, float screen_y)
{
  m_current_tool->on_secondary_button_release(screen_x, screen_y);
}

void
WorldViewComponent::on_delete_press(float screen_x, float screen_y)
{
  m_current_tool->on_delete_press(screen_x, screen_y);
}

void
WorldViewComponent::on_duplicate_press(float screen_x, float screen_y)
{
  m_current_tool->on_duplicate_press(screen_x, screen_y);
}

void
WorldViewComponent::on_join_press(float x, float y)
{
  m_current_tool->on_join_press(x, y);
}

void
WorldViewComponent::on_fix_press(float screen_x, float screen_y)
{
  m_current_tool->on_fix_press(screen_x, screen_y);
}

void
WorldViewComponent::scroll_left()
{
  m_zoom.translate_offset(-20, 0);
}

void
WorldViewComponent::scroll_right()
{
  m_zoom.translate_offset(20, 0);
}

void
WorldViewComponent::scroll_up()
{
  m_zoom.translate_offset(0, -20);
}

void
WorldViewComponent::scroll_down()
{
  m_zoom.translate_offset(0, 20);
}

void
WorldViewComponent::on_tertiary_button_press(float x, float y)
{
  m_scrolling = true;
  g_graphic_context->push_cursor();
  g_graphic_context->set_cursor(CURSOR_SCROLL);

  m_x_offset = m_zoom.get_x_offset();
  m_y_offset = m_zoom.get_y_offset();
  WorldGUIManager::instance()->grab_mouse(this);

  m_scroll_pos_x = m_zoom.screen_to_world_x(x);
  m_scroll_pos_y = m_zoom.screen_to_world_y(y);
}

void
WorldViewComponent::on_tertiary_button_release(float x, float y)
{
  g_graphic_context->pop_cursor();
  m_scrolling = false;
  WorldGUIManager::instance()->ungrab_mouse(this);
}

void
WorldViewComponent::on_mouse_move(float x, float y, float of_x, float of_y)
{
  if (m_scrolling)
  {
    float const new_scroll_pos_x = x / m_zoom.get_zoom() - m_x_offset;
    float const new_scroll_pos_y = y / m_zoom.get_zoom() - m_y_offset;

    m_zoom.set_offset(m_x_offset + (new_scroll_pos_x - m_scroll_pos_x),
                      m_y_offset + (new_scroll_pos_y - m_scroll_pos_y));

  }
  else
  {
    m_current_tool->on_mouse_move (x, y, of_x, of_y);
  }
}

void
WorldViewComponent::on_scale_press(float x, float y)
{
  m_current_tool->on_scale_press(x,y);
}

void
WorldViewComponent::on_grid_press(float x, float y)
{
  m_use_grid = !m_use_grid;
}

float
WorldViewComponent::get_zoom ()
{
  return m_zoom.get_zoom();
}

void
WorldViewComponent::on_world_change()
{
  World& world = *Controller::instance()->get_world();

  const BoundingBox& box = world.calc_bounding_box();
  // Zoom to the bounding box
  m_zoom.zoom_to(static_cast<int>(box.x1), static_cast<int>(box.y1),
               static_cast<int>(box.x2), static_cast<int>(box.y2));
  // Zoom out two times so that the area isn't covered up by the
  // GUI
  m_zoom.zoom_out(get_width()/2, get_height()/2);
  m_zoom.zoom_out(get_width()/2, get_height()/2);
}

/* EOF */
