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

#ifndef HEADER_GUI_DIRECTORY_BUTTON_HPP
#define HEADER_GUI_DIRECTORY_BUTTON_HPP

#include "gui_file_button.hpp"

namespace construo {

class GUIDirectoryButton : public GUIFileButton
{
public:
  GUIDirectoryButton(const std::string& pathname, std::function<void ()> callback);
  ~GUIDirectoryButton();

  void draw(GraphicContext& gc) override;
  void on_click() override;

private:
  std::function<void ()> m_callback;
};

} // namespace construo

#endif

/* EOF */
