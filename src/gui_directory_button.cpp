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

#include "gui_file_manager.hpp"
#include "gui_directory_button.hpp"

GUIDirectoryButton::GUIDirectoryButton (const std::string& arg_filename)
  : GUIFileButton (arg_filename)
{
}

GUIDirectoryButton::~GUIDirectoryButton ()
{
}

void
GUIDirectoryButton::draw (GraphicContext* parent_gc)
{
  //std::cout << "GUIDirectoryButton: " << filename << std::endl;
  parent_gc->draw_fill_rect (m_x_pos, m_y_pos,
                             m_x_pos +  m_width, m_y_pos + m_height,
                             Color (0xBB0000FF));

  parent_gc->draw_string (m_x_pos + 40, m_y_pos + 20, "..:: Directory ::..");
  parent_gc->draw_string (m_x_pos + 30, m_y_pos + 40, filename);

  if (mouse_over)
    parent_gc->draw_rect (m_x_pos, m_y_pos,
                          m_x_pos +  m_width, m_y_pos + m_height,
                          Color (0xFFFFFFFF));
  else
    parent_gc->draw_rect (m_x_pos, m_y_pos,
                          m_x_pos +  m_width, m_y_pos + m_height,
                          Color (0xFF0000FF));
}

void
GUIDirectoryButton::on_click()
{
  std::cout << "Click on GUIDirectoryButton detected" << std::endl;
  GUIFileManager::instance()->open_directory(filename);
}

/* EOF */
