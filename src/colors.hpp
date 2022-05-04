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

#ifndef HEADER_CONSTRUO_COLORS_HPP
#define HEADER_CONSTRUO_COLORS_HPP

#include "color.hpp"

struct GUIStyle
{
  Color button_fg_pressed;
  Color button_fg_hover;
  Color button_fg_passive;

  Color button_bg_pressed;
  Color button_bg_hover;
  Color button_bg_passive;
  Color button_bg_active;

  /** Color for new springs and other helper lines */
  Color new_spring;

  Color selection_rect;
  Color selection_resizer;

  /** Color for collidables */
  Color rect_collider_fg;
  Color rect_collider_bg;
  Color ground_color;
  Color ground_grid_color;

  /** Color for highlighted particles or springs */
  Color highlight;

  /** Color of the background grid */
  Color grid_color;
  Color grid_color2;
};

extern GUIStyle const g_style;

#endif

/* EOF */
