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

#include "construo.hpp"
#include "system_context.hpp"
#include "world_button.hpp"
#include "gui_directory_button.hpp"
#include "gui_file_manager.hpp"
#include "gui_new_file_button.hpp"
#include "gui_directory.hpp"

GUIDirectory::GUIDirectory (const std::string& arg_pathname, Mode m)
  : GUIChildManager (0, 0, 800, 600),
    pathname (arg_pathname),
    mode (m)
{
  mtime = g_system_context->get_mtime(pathname);
  std::vector<std::string> dir = g_system_context->read_directory(pathname);

  if (mode == SAVE_DIRECTORY && pathname != "/")
    files.push_back(new GUINewFileButton(pathname));

  for (std::vector<std::string>::iterator i = dir.begin(); i != dir.end(); ++i)
    {
      std::string filename = pathname + *i;

      FileType type = g_system_context->get_file_type (filename);

      //std::cout << "Creating object for: " << filename << std::endl;

      if (type == FT_DIRECTORY)
        {
          if (*(filename.end()-1) == '/') // FIXME: Hack
            files.push_back (new GUIDirectoryButton (filename));
          else
            files.push_back (new GUIDirectoryButton (filename + "/"));
        }
      else if (type == FT_CONSTRUO_FILE)
        {
          if (mode == SAVE_DIRECTORY)
            files.push_back (new WorldButton (filename, WorldButton::SAVE_BUTTON));
          else
            files.push_back (new WorldButton (filename, WorldButton::LOAD_BUTTON));
        }
      else // (type == FT_UNKNOWN_FILE)
        {
          // ignore unknown files
          std::cout << "GUIFileManager: ignoring '" << filename
                    << "' since it has unknown filetype" << std::endl;
        }
    }

  offset = 0;
  place_components ();
}

GUIDirectory::~GUIDirectory ()
{
  for(std::vector<GUIFileButton*>::iterator i = files.begin();
      i != files.end(); ++i)
    {
      // FIXME: Very ugly, we remove all components from the manager so that he doesn't delete them twice
      remove(*i);
      delete *i;
    }
}

void
GUIDirectory::place_components ()
{
  // Remove all file components
  for(std::vector<GUIFileButton*>::iterator i = files.begin();
      i != files.end(); ++i)
    {
      remove(*i);
    }

  int row = 0;
  int column = 0;
  int count = 0;

  //std::cout << "OFFSET: " << offset << std::endl;

  for(std::vector<GUIFileButton*>::size_type i = 0 + offset;
      i < files.size() && count < 9;
      ++i)
    {
      files[i]->set_position(column * (200 + 50) + 50,
                             row * (150 + 37) + 30);
      add(files[i]);

      column += 1;
      if (column >= 3) // row is full
        {
          column = 0;
          row += 1;
        }
      if (row >= 3)
        return;

      ++count;
    }
}

void
GUIDirectory::draw_overlay (GraphicContext* gc)
{
}

void
GUIDirectory::move_up ()
{
  if (offset >= 3)
    offset -= 3;

  place_components ();
}

void
GUIDirectory::move_down ()
{
  offset += 3;

  if (offset >= int(files.size()))
    offset -= 3;

  place_components ();
}

void
GUIDirectory::wheel_up (int x, int y)
{
  move_up();
}

void
GUIDirectory::wheel_down (int x, int y)
{
  move_down();
}

/* EOF */
