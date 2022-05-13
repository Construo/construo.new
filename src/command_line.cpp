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

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "settings.hpp"
#include "command_line.hpp"

void
CommandLine::error (const std::string& msg)
{
  std::cerr << "CommandLine parse failure!" << std::endl;
  std::cerr << "CommandLine: " << msg << std::endl;
  exit(EXIT_FAILURE);
}

void
CommandLine::parse (int argc, char** argv)
{
  // FIXME: Replace this with getopt_long() or argp()
  for (int i = 1; i < argc; ++i) // Skip Programm name
    {
      if (argv[i][0] == '-') // Argument is an option
        {
          if (strcmp(argv[i], "--fullscreen") == 0
              || strcmp(argv[i], "-f") == 0)
            {
              g_settings.fullscreen = true;
            }
          else if (strcmp(argv[i], "--version") == 0
                   || strcmp(argv[i], "-v") == 0)
            {
              print_version ();
              exit (EXIT_SUCCESS);
            }
          else if (strcmp(argv[i], "--help") == 0
              || strcmp(argv[i], "-h") == 0)
            {
              print_help(std::cout);
              exit (EXIT_SUCCESS);
            }
          else if (strcmp(argv[i], "--disable-alphablending") == 0)
            {
              g_settings.alphablending = false;
            }
          else if (strcmp(argv[i], "--disable-antialiasing") == 0
                   || strcmp(argv[i], "-d") == 0)
            {
              g_settings.antialiasing = false;
            }
          else if (strcmp(argv[i], "--disable-thicklines") == 0
                   || strcmp(argv[i], "-l") == 0)
            {
              g_settings.thick_lines = false;
            }
          else if (strcmp(argv[i], "--disable-doublebuffer") == 0)
            {
              g_settings.doublebuffer = false;
            }
          else if (strcmp(argv[i], "--datadir") == 0)
            {
              g_settings.datadir = argv[++i];
            }
          else if (strcmp(argv[i], "-g") == 0
                   || strcmp(argv[i], "--geometry") == 0)
            {
              if (++i < argc)
                {
                  char c;
                  if (sscanf(argv[i], "%d%c%d",
                             &g_settings.screen_width, &c, &g_settings.screen_height) != 3 && c != 'x')
                    {
                      error ("geometry string must WIDTHxHEIGHT");
                    }
                }
              else
                {
                  error ("-g/--geometry is missing an argument");
                }
            }
          else
            {
              error (std::string("Unknown option: ") + argv[i]);
            }
        }
      else
        {
          if (!g_settings.startup_file.empty())
            {
              error ("Only one filename can be given");
            }
          else
            {
              g_settings.startup_file = argv[i];
            }
        }
    }
}

void
CommandLine::print_help(std::ostream& out)
{
  out << "Construo " VERSION "\n\n"
      << "Usage: construo [OPTIONS] [FILENAME]\n\n"
      << "  -h, --help                  display this help text\n"
      << "  -v,--version                Print version number of the programm\n"
      << "  -f, --fullscreen            switch to fullscreen mode if available\n"
      << "  -g, --geometry WIDTHxHEIGHT switch resolution to WIDTH and HEIGHT\n"
      << "  -d, --disable-antialiasing  Switch into non antialiased mode\n"
      << "  -l, --disable-thicklines    Disable thick (slow) line drawing\n"
      << "  --disable-doublebuffer  disable the double buffer (might cause flicker)\n"
      << "  --disable-alphablending     Switch into non alphablended mode\n"
      << "  --datadir DIR               Set the datadir of Construo to DIR\n"
      << std::endl;
}

void
CommandLine::print_version ()
{
  std::cout << "Construo " VERSION "\n"
            << "Written by Ingo Ruhnke <grumbel@gmail.com>\n\n"
            << "Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>\n"
            << "This is free software; see the source for copying conditions.  There is NO\n"
            << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE\n";
}

/* EOF */

