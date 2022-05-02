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

#ifndef HEADER_CONSTRUO_WORLDVIEW_TOOL_HPP
#define HEADER_CONSTRUO_WORLDVIEW_TOOL_HPP

class ZoomGraphicContext;

/** */
class WorldViewTool
{
public:
  WorldViewTool () {}

  virtual ~WorldViewTool () {}

  /** Let the Tool to some initialisation */
  virtual void activate () {}

  /** Let the Tool to some deinitialisation */
  virtual void deactivate () {}

  virtual void draw_background (ZoomGraphicContext* gc) =0;
  virtual void draw_foreground (ZoomGraphicContext* gc) =0;

  virtual void on_button_press (int button_id, float x, float y) {}

  virtual void on_primary_button_press (float x, float y) {}
  virtual void on_primary_button_release (float x, float y) {}

  virtual void on_secondary_button_press (float x, float y) {}
  virtual void on_secondary_button_release (float x, float y) {}

  virtual void on_scale_press (float x, float y) {}
  virtual void on_duplicate_press (float x, float y) {}
  virtual void on_delete_press (float x, float y) {}
  virtual void on_fix_press (float x, float y) {}
  virtual void on_join_press (float x, float y) {}

  virtual void on_mouse_move (float x, float y, float of_x, float of_y) {}
};

#endif

/* EOF */
