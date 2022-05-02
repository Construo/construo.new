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

#ifndef HEADER_CONSTRUO_GLUT_DISPLAY_HPP
#define HEADER_CONSTRUO_GLUT_DISPLAY_HPP

#include "root_graphic_context.hpp"
#include "input_context.hpp"

class GlutDisplay : public RootGraphicContext,
                    public InputContext
{
private:
  static GlutDisplay* instance_;

public:
  static GlutDisplay* instance() { return instance_; }

public:
  GlutDisplay(int w, int h, int fullscreen);
  virtual ~GlutDisplay();

  void draw_lines(std::vector<Line>& lines, Color color, int wide = 0) override;
  void draw_line(float x1, float y1, float x2, float y2, Color color, int wide = 0) override;
  void draw_rect(float x1, float y1, float x2, float y2, Color color) override;
  void draw_fill_rect(float x1, float y1, float x2, float y2, Color color) override;
  void draw_circle(float x, float y, float r, Color color) override;
  void draw_circles(std::vector<Circle>& circles, Color color) override;
  void draw_fill_circle(float x, float y, float r, Color color) override;
  void draw_string(float x, float y, const std::string& str, Color color) override;
  void draw_string_centered(float x, float y, const std::string& str, Color color) override;

  void clear() override;
  void flip() override;

  bool get_fullscreen() { return m_is_fullscreen; }

  float get_width() override { return static_cast<float>(m_width); }
  float get_height() override { return static_cast<float>(m_height); }

  bool get_key(int key) override;
  float get_mouse_x() override;
  float get_mouse_y() override;

  void run();
  void set_cursor_real(CursorType) override;
  void reshape_func(int w, int h);
  void display_func();
  void mouse_func(int button, int button_state, int x, int y);
  void idle_func();
  void keyboard_func(unsigned char key, int x, int y);
  void special_func(int key, int x, int y);
  void mouse_motion_func(int x, int y);

  void set_clip_rect(float x1, float y1, float x2, float y2) override;

  void push_quick_draw() override;
  void pop_quick_draw() override;

  void enter_fullscreen() override;
  void leave_fullscreen() override;

private:
  /** X-Position of the glut window, used in fullscreen to allow a
      restore */
  int m_window_x_pos;

  /** Y-Position of the glut window, used in fullscreen to allow a
      restore */
  int m_window_y_pos;

  /** Width of the window, used to restore after fullscreen */
  int m_window_width;

  /** height of the window, used to restore after fullscreen */
  int m_window_height;

  /** The actual size of the current screen/window */
  int m_width;

  /** The actual size of the current screen/window */
  int m_height;

  int m_mouse_x;
  int m_mouse_y;

  bool m_block;

  int m_update_display;

  bool m_is_fullscreen;

public:
  GlutDisplay(const GlutDisplay&) = delete;
  GlutDisplay& operator=(const GlutDisplay&) = delete;
};

#endif

/* EOF */
