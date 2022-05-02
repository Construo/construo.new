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

namespace Colors {

Color button_fg_pressed (0xFFFFFFFF);
Color button_fg_hover   (0x009f9fFF);
Color button_fg_passive (0x007f7fFF);

Color button_bg_pressed (0x000000FF);
Color button_bg_hover   (0x006060EE);
Color button_bg_passive (0x004f4fAA);
Color button_bg_active  (0x00AAAAAA);

Color new_spring        (0xAAAAAAFF);
Color selection_rect    (0xFFFFFFFF);
Color selection_resizer (0xFFFF00FF);

Color rect_collider_fg  (0x0000FFFF);
Color rect_collider_bg  (0x0000AAFF);
Color highlight         (0xEEEEEEFF);

Color grid_color        (0x444444FF);
Color grid_color2       (0x666666FF);
Color ground_color      (0x000044FF);
Color ground_grid_color (0x000073FF);

} // namespace Colors

/* EOF */
