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

#if defined(HAVE_FREEGLUT)
#  include <GL/freeglut.h>
#elif defined(__APPLE__)
#  include <glut.h>
#else
#  include <GL/glut.h>
#endif

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include "buttons.hpp"
#include "events.hpp"
#include "settings.hpp"
#include "construo_main.hpp"
#include "screen_manager.hpp"
#include "glut_display.hpp"

GlutDisplay* GlutDisplay::instance_ = nullptr;

void reshape_func(int w, int h)
{
  GlutDisplay::instance()->reshape_func(w, h);
}

void display_func ()
{
  GlutDisplay::instance()->display_func();
}

void mouse_func (int button, int button_state, int x, int y)
{
  GlutDisplay::instance()->mouse_func(button, button_state, x, y);
}

void idle_func ()
{
  GlutDisplay::instance()->idle_func();
}

void keyboard_func (unsigned char key, int x, int y)
{
  GlutDisplay::instance()->keyboard_func(key, x, y);
}

void special_func (int key, int x, int y)
{
  GlutDisplay::instance()->special_func(key, x, y);
}

void mouse_motion_func (int x, int y)
{
  GlutDisplay::instance()->mouse_motion_func(x, y);
}

GlutDisplay::GlutDisplay (int w, int h, int fullscreen) :
  window_x_pos(),
  window_y_pos(),
  window_width(),
  window_height(),
  width(),
  height(),
  mouse_x(),
  mouse_y(),
  block(),
  update_display(),
  is_fullscreen()
{
  instance_ = this;

  update_display = 0;
  width  = w;
  height = h;

  int argc = 1;
  char* argv[2];
  argv[0] = strdup("construo");
  argv[1] = NULL;
  glutInit(&argc, argv); // Only pass empty dummies
  free(argv[0]);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(width, height);
  //glutInitWindowPosition(100, 100); don't care
  glutSetWindow(glutCreateWindow(construo_main->get_title()));

  glutDisplayFunc(::display_func);
  glutReshapeFunc(::reshape_func);
  glutMouseFunc(::mouse_func);

  glutMotionFunc (::mouse_motion_func);
  glutPassiveMotionFunc (::mouse_motion_func);

  glutIdleFunc (::idle_func);
  glutKeyboardFunc(::keyboard_func);
  glutSpecialFunc(::special_func);

  glClearColor (0.0f, 0.0f, 0.0f, 0.1f);
  if (settings.alphablending)
  {
    glShadeModel (GL_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  if (settings.antialiasing && settings.alphablending)
  {
    glEnable(GL_LINE_SMOOTH);
  }

  //glEnable(GL_SCISSOR_TEST);
  //glScissor(0, 0, settings.screen_width, settings.screen_height);
  glutSetCursor(GLUT_CURSOR_FULL_CROSSHAIR);

  window_x_pos = 0;
  window_y_pos = 0;
  window_width  = w;
  window_height = h;

  if (fullscreen)
    enter_fullscreen();
}

void
GlutDisplay::run()
{
  std::cout << "Starting glut mainloop" << std::endl;
  glutMainLoop();
  std::cout << "Ending glut mainloop" << std::endl;
}

GlutDisplay::~GlutDisplay()
{
}

void
GlutDisplay::draw_lines (std::vector<Line>& lines, Color color, int wide)
{
  if (settings.thick_lines)
    glLineWidth(static_cast<float>(wide));

  glBegin (GL_LINES);
  for (std::vector<Line>::const_iterator i = lines.begin(); i != lines.end(); ++i)
    {
      glVertex2f (i->x1, i->y1);
      glVertex2f (i->x2, i->y2);
    }
  glEnd ();
}

void
GlutDisplay::draw_line(float x1, float y1, float x2, float y2, Color color, int wide)
{
  if (settings.thick_lines)
    glLineWidth(static_cast<float>(wide));

  glColor4f (color.r, color.g, color.b, color.a);
  glBegin (GL_LINES);
  glVertex2f (x1, y1);
  glVertex2f (x2, y2);
  glEnd ();
}

void
GlutDisplay::draw_rect(float x1, float y1, float x2, float y2, Color color)
{
  if (settings.thick_lines)
    glLineWidth (2);

  glColor4f (color.r, color.g, color.b, color.a);
  glBegin (GL_LINE_STRIP);
  glVertex2f (x1, y1);
  glVertex2f (x2, y1);
  glVertex2f (x2, y2);
  glVertex2f (x1, y2);
  glVertex2f (x1, y1);
  glEnd ();
}

void
GlutDisplay::draw_fill_rect(float x1, float y1, float x2, float y2, Color color)
{
  if (settings.thick_lines)
    glLineWidth (.5f);

  glColor4f (color.r, color.g, color.b, color.a);
  glBegin (GL_QUADS);
  glVertex2f (x1, y1);
  glVertex2f (x2, y1);
  glVertex2f (x2, y2);
  glVertex2f (x1, y2);
  glEnd ();
}

void
GlutDisplay::draw_circles(std::vector<Circle>& circles, Color color)
{
  for (std::vector<Circle>::iterator i = circles.begin(); i != circles.end(); ++i)
    {
      draw_circle(i->x, i->y, i->r, color);
    }
}

void
GlutDisplay::draw_circle(float x, float y, float r, Color color)
{
  glColor4f (color.r, color.g, color.b, color.a);

  GLUquadricObj* qobj = gluNewQuadric ();
  gluQuadricDrawStyle(qobj, GLU_SILHOUETTE);
  //gluQuadricNormals (qobj, GLU_FLAT);
  glPushMatrix();
  glTranslatef (x, y, 0);
  gluDisk (qobj, 0, r, 8, 1);
  /*draw_rect (x - r, y - r, x + r, y + r,
    color);*/
  glPopMatrix ();
  gluDeleteQuadric (qobj);
}

void
GlutDisplay::draw_fill_circle(float x, float y, float r, Color color)
{
  glColor4f (color.r, color.g, color.b, color.a);
  //draw_fill_rect (x - r, y - r, x + r, y + r,
  //              color);

  GLUquadricObj* qobj = gluNewQuadric ();
  gluQuadricDrawStyle(qobj, GLU_FILL);
  //gluQuadricNormals (qobj, GLU_FLAT);
  glPushMatrix();
  glTranslatef (x, y, 0);
  gluDisk (qobj, 0, r, 8, 1);
  /*draw_rect (x - r, y - r, x + r, y + r,
    color);*/
  glPopMatrix ();
  gluDeleteQuadric (qobj);
}

void
GlutDisplay::draw_string(float x, float y, const std::string& str, Color color)
{
  if (settings.thick_lines)
    glLineWidth (1.0f);

  glColor4f (color.r, color.g, color.b, color.a);
  glPushMatrix();
  glTranslatef (x , y, 0);
  glScalef (.07f, -.07f, 0);

  for (std::string::const_iterator i = str.begin (); i != str.end (); ++i)
    {
      glutStrokeCharacter  (GLUT_STROKE_MONO_ROMAN, *i);
      //glutStrokeWidth (GLUT_STROKE_MONO_ROMAN, *i);
    }
  glPopMatrix();
}


void
GlutDisplay::draw_string_centered(float x, float y, const std::string& str, Color color)
{
  draw_string(x - (7.5f * static_cast<float>(str.length())) / 2.0f,
              y, str, color);
}

bool
GlutDisplay::get_key (int key)
{
  return false;
}

int
GlutDisplay::get_mouse_x ()
{
  return mouse_x;
}

int
GlutDisplay::get_mouse_y ()
{
  return mouse_y;
}

void
GlutDisplay::clear ()
{
  glClear(GL_COLOR_BUFFER_BIT);
}

void
GlutDisplay::flip ()
{
  glutSwapBuffers();
}

void
GlutDisplay::flip (int x1, int y1, int x2, int y2)
{
  assert (false);
}


void
GlutDisplay::reshape_func(int w, int h)
{
  std::cout << "Reshape: " << w << " " << h << std::endl;
  glViewport (0,0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D (0, w, h, 0);
  glMatrixMode(GL_MODELVIEW);
  width  = w;
  height = h;
}

void
GlutDisplay::display_func ()
{
  ScreenManager::instance()->run_once();
}

void
GlutDisplay::mouse_func (int button, int button_state, int x, int y)
{
  mouse_x = x;
  mouse_y = y;

  Event event;
  event.type = BUTTON_EVENT;

  //std::cout << "mouse button press: " << button << " " << button_state <<  " " << x << " " << y << std::endl;

  if (button_state == 0)
    event.button.pressed = true;
  else
    event.button.pressed = false;

  switch (button)
    {
    case 0:
      event.button.id = BUTTON_PRIMARY;
      break;
    case 1:
      event.button.id = BUTTON_TERTIARY;
      break;
    case 2:
      event.button.id = BUTTON_SECONDARY;
      break;
    case 3:
      event.button.id = BUTTON_ZOOM_IN;
      break;
    case 4:
      event.button.id = BUTTON_ZOOM_OUT;
      break;
    default:
      std::cout << "GlutDisplay: Unhandle mouse button press: " << button << " " << button_state << std::endl;
      return;
    }
  events.push (event);
}

void
GlutDisplay::idle_func ()
{
  /*  if (Controller::instance()->is_running() || update_display > 0)
      {
      //system_context->sleep (0); // limit CPU usage via brute force
      update_display = 0;
      }*/
  if (!ScreenManager::instance ()->is_finished())
    {
      ScreenManager::instance ()->run_once();
    }
  else
    {
      construo_main->exit();
    }
}

void
GlutDisplay::special_func (int key, int x, int y)
{
  switch (key)
    {
    case GLUT_KEY_F11:
      if (is_fullscreen)
        leave_fullscreen();
      else
        enter_fullscreen();
      break;
    }
}

void
GlutDisplay::keyboard_func (unsigned char key, int x, int y)
{
  //std::cout << "GlutDisplay: keypress: " << key << " (" << int(key) << ") " << x << " " << y << std::endl;

  Event event;
  event.type = BUTTON_EVENT;
  event.button.pressed = true;

  switch (key)
    {
    case 127: // Delete
      event.button.id = BUTTON_DELETE;
      break;
    case 32: // Space
      event.button.id = BUTTON_RUN;
      break;
    case 9: // Tab
      event.button.id = BUTTON_TOGGLESLOWMO;
      break;
    case 27: // Escape
    case 'q':
      event.button.id = BUTTON_ESCAPE;
      break;
    case 'h':
      event.button.id = BUTTON_FLIP;
      break;
    case 'f':
      event.button.id = BUTTON_FIX;
      break;
    case 'd':
      event.button.id = BUTTON_DUPLICATE;
      break;

    case 'v':
      event.button.id = BUTTON_SETVELOCITY;
      break;

    case 'c':
      event.button.id = BUTTON_CLEAR;
      break;

    case 'j':
      event.button.id = BUTTON_JOIN;
      break;

    case 's':
      event.button.id = BUTTON_SCALE;
      break;

    case 'a':
      event.button.id = BUTTON_ACTIONCAM;
      break;

    case 'o':
      event.button.id = BUTTON_HIDEDOTS;
      break;

    case '!':
      event.button.id = BUTTON_QUICKLOAD1;
      break;

    case '@':
      event.button.id = BUTTON_QUICKLOAD2;
      break;

    case '#':
      event.button.id = BUTTON_QUICKLOAD3;
      break;

    case '$':
      event.button.id = BUTTON_QUICKLOAD4;
      break;

    case '%':
      event.button.id = BUTTON_QUICKLOAD5;
      break;

    case '^':
      event.button.id = BUTTON_QUICKLOAD6;
      break;

    case '&':
      event.button.id = BUTTON_QUICKLOAD7;
      break;

    case '*':
      event.button.id = BUTTON_QUICKLOAD8;
      break;

    case '(':
      event.button.id = BUTTON_QUICKLOAD9;
      break;

    case ')':
      event.button.id = BUTTON_QUICKLOAD0;
      break;

    case '0':
      event.button.id = BUTTON_QUICKSAVE0;
      break;

    case '1':
      event.button.id = BUTTON_QUICKSAVE1;
      break;

    case '2':
      event.button.id = BUTTON_QUICKSAVE2;
      break;

    case '3':
      event.button.id = BUTTON_QUICKSAVE3;
      break;

    case '4':
      event.button.id = BUTTON_QUICKSAVE4;
      break;

    case '5':
      event.button.id = BUTTON_QUICKSAVE5;
      break;

    case '6':
      event.button.id = BUTTON_QUICKSAVE6;
      break;

    case '7':
      event.button.id = BUTTON_QUICKSAVE7;
      break;

    case '8':
      event.button.id = BUTTON_QUICKSAVE8;
      break;

    case '9':
      event.button.id = BUTTON_QUICKSAVE9;
      break;

    case 'g':
      event.button.id = BUTTON_GRID;
      break;

    case 'u':
      event.button.id = BUTTON_UNDO;
      break;
    case 'r':
      event.button.id = BUTTON_REDO;
      break;
    case '+':
    case '=': // so that people don't have to press shift
      event.button.id = BUTTON_ZOOM_IN;
      break;
    case '-':
      event.button.id = BUTTON_ZOOM_OUT;
      break;
    default:
      //std::cout << "GlutDisplay: Unhandled keypress: '" << key << "'[" << int(key) << "] x/y: "
      //        << x << ", " << y << std::endl;
      return;
    }

  events.push(event);
}

void
GlutDisplay::mouse_motion_func (int x, int y)
{
  //std::cout << "Motion: " << x << " " << y << std::endl;
  mouse_x = x;
  mouse_y = y;
}

void
GlutDisplay::leave_fullscreen()
{
  std::cout << "GlutDisplay: leaving fullscreen: restoring to: pos: "
            << window_x_pos << ", " << window_y_pos << " - WxH: "
            << window_width << ", " << window_height << std::endl;

  glutReshapeWindow(window_width, window_height);
  glutPositionWindow(window_x_pos, window_y_pos);

  is_fullscreen = false;
}

void
GlutDisplay::enter_fullscreen()
{
#if 0
  char mode[64];
  snprintf (mode, 64, "%dx%d:%d@%d", width, height, 16, 80);
  std::cout << "GlutDisplay: switching to: " << mode << std::endl;
  glutGameModeString(mode);
  glutEnterGameMode();
  is_fullscreen = true;
#else
  std::cout << "GlutDisplay: Entering fullscreen" << std::endl;

  window_x_pos  = glutGet((GLenum)GLUT_WINDOW_X);
  window_y_pos  = glutGet((GLenum)GLUT_WINDOW_Y);
  window_width  = glutGet((GLenum)GLUT_WINDOW_WIDTH);
  window_height = glutGet((GLenum)GLUT_WINDOW_HEIGHT);

  std::cout << "Saving window: " << window_x_pos << ", " << window_y_pos << " - WxH: "
            << window_width << ", " << window_height << std::endl;

  glutFullScreen();

  is_fullscreen = true;
#endif
}

void
GlutDisplay::set_clip_rect (int x1, int y1, int x2, int y2)
{
  //std::cout << "Setting cliprect: " << x1<< " " <<y1<< " " <<x2-x1+1<< " " <<y2-y1+1<<std::endl;
  // FIXME: doesn't really work for some reason
  //std::cout << "Clip: " <<  x1 << ", " << y1 << " - " <<  x2-x1+1 << "x" << y2-y1+1 << std::endl;
  //glScissor(x1, y1, x2-x1+1, y2-y1+1);
}

void
GlutDisplay::push_quick_draw()
{
  if (settings.antialiasing && settings.alphablending)
    {
      glDisable(GL_LINE_SMOOTH);
    }
}

void
GlutDisplay::pop_quick_draw()
{
  if (settings.antialiasing && settings.alphablending)
    {
      glEnable(GL_LINE_SMOOTH);
    }
}

void
GlutDisplay::set_cursor_real(CursorType cursor)
{
  switch(cursor)
    {
    case CURSOR_INSERT:
      glutSetCursor(GLUT_CURSOR_CROSSHAIR);
      break;
    case CURSOR_SCROLL:
      glutSetCursor(GLUT_CURSOR_INFO);
      break;
    case CURSOR_ZOOM:
      glutSetCursor(GLUT_CURSOR_HELP);
      break;
    case CURSOR_COLLIDER:
      glutSetCursor(GLUT_CURSOR_CROSSHAIR);
      break;
    case CURSOR_SELECT:
      glutSetCursor(GLUT_CURSOR_INHERIT);
      break;
    case CURSOR_ROTATE:
      glutSetCursor(GLUT_CURSOR_CYCLE);
      break;
    default:
      std::cout << "GlutDisplay: Unhandled cursor type: " << static_cast<int>(cursor) << std::endl;
      break;
    }

}

/* EOF */
