//  $Id$
//
//  Construo - A wire-frame construction game
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <fstream>
#include "construo.hxx"
#include "particle.hxx"
#include "config.h"
#include "world.hxx"
#include "construo_error.hxx"
#include "x11_display.hxx"
#include "unix_system.hxx"
#include "controller.hxx"
#include "command_line.hxx"
#include "settings.hxx"
#include "gui_manager.hxx"

#include "construo_main.hxx"

ConstruoMain* construo_main;
Controller*   controller;
GUIManager*   gui_manager;

ConstruoMain::ConstruoMain ()
{
  do_quit = false;
}

ConstruoMain::~ConstruoMain ()
{
}

char* 
ConstruoMain::get_title ()
{
  return "Construo";
}

void
ConstruoMain::on_exit()
{
  if (!controller->has_been_run())
    controller->save_world(system_context->get_construo_rc_path() + "laststate.construo");
}

int 
ConstruoMain::main (int argc, char* argv[]) // FIXME: pass an option class, instead command line arguments
{
  //FIXME:slot_press = CL_Input::sig_button_press ().connect (this, &ConstruoMain::on_press);
  //FIXME:slot_release = CL_Input::sig_button_release ().connect (this, &ConstruoMain::on_release);

  gui_manager = new GUIManager ();

  if (argc == 2)
    {
      controller  = new Controller (argv[1]);
    }
  else
    {
      try 
        {
          controller = new Controller (system_context->get_construo_rc_path() + "laststate.construo");
        } 
      catch (ConstruoError& err) 
        {
          std::cout << "ConstruoMain: " << err.msg << std::endl;
          controller = new Controller ();
        }
    }


  gui_manager->run ();

  on_exit();

  return 0;
}

////////////////////////
// Real Main Function //
////////////////////////
int main (int argc, char** argv)
{
  CommandLine::parse(argc, argv);

  if (argc == 2 && argv[1][0] == '-')
    {
      std::cout << "Usage: " << argv[0] << " [FILENAME]" << std::endl;
      return EXIT_FAILURE;
    }

  try {
    X11Display display (settings.screen_width, settings.screen_height, 
                        settings.fullscreen);
    UnixSystem system;

    // Init the display, input systems
    graphic_context = &display;
    input_context   = &display;
    system_context  = &system;
  
    std::cout << PACKAGE_STRING"\n" << std::endl;
    std::cout << "If you have throuble with programm startup, delete the file:\n\n" 
              << "    " << system_context->get_construo_rc_path() << "laststate.construo\n" << std::endl;

    ConstruoMain app;
    construo_main = &app;
    
    int ret_val = app.main (argc, argv);

    return ret_val;
  } catch (ConstruoError& err) {
    std::cout << "Error ocurred: " << err.msg << std::endl;
    return EXIT_FAILURE;
  }
}

/* EOF */
