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

#ifndef HEADER_X11_DISPLAY_HPP
#define HEADER_X11_DISPLAY_HPP

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#ifdef HAVE_LIBXXF86VM
#  include <X11/extensions/xf86vmode.h>
#endif

#include "math.hpp"
#include "root_graphic_context.hpp"
#include "input_context.hpp"

#define X11_FULLSCREEN_MODE true
#define X11_WINDOW_MODE     false

struct FlipRect
{
  int x1;
  int y1;
  int x2;
  int y2;
};

/** X11Display driver */
class X11Display : public RootGraphicContext,
                   public InputContext
{
private:
  Cursor cursor_scroll;
  Pixmap cursor_scroll_pix;
  Pixmap cursor_scroll_mask;

  Cursor cursor_zoom;
  Pixmap cursor_zoom_pix;
  Pixmap cursor_zoom_mask;

  Cursor cursor_insert;
  Pixmap cursor_insert_pix;
  Pixmap cursor_insert_mask;

  Cursor cursor_select;
  Pixmap cursor_select_pix;
  Pixmap cursor_select_mask;

  Cursor cursor_collider;
  Pixmap cursor_collider_pix;
  Pixmap cursor_collider_mask;

  bool doublebuffer;
#ifdef HAVE_LIBXXF86VM
  XF86VidModeModeLine orig_modeline;
#endif
  int orig_viewport_x;
  int orig_viewport_y;
  int orig_dotclock;

  int       width;
  int       height;
  Display*  display;
  Window    window;
  Colormap  colormap;
  Drawable  drawable;
  GC        gc;

  bool shift_pressed;
  int  mouse_x;
  int  mouse_y;

  /** Color Depth of the Display */
  int depth;

  /** true if display is in fullscreen mode, false for window mode */
  bool fullscreen;

  std::vector<FlipRect> flip_rects;
  std::vector<FlipRect> last_flip_rects;

  /** Save the current visual mode for later restoration after leaving
      fullscreen */
  void save_mode();
public:
  X11Display (int w, int h, bool fullscreen_);
  virtual ~X11Display ();

  // Graphic Context stuff
  void draw_lines (std::vector<Line>& lines, Color color, int wide = 0) override;
  void draw_line(float x1, float y1, float x2, float y2, Color color, int wide = 0) override;
  void draw_rect(float x1, float y1, float x2, float y2, Color color) override;
  void draw_fill_rect(float x1, float y1, float x2, float y2, Color color) override;
  void draw_circle(float x, float y, float r, Color color) override;
  void draw_circles(std::vector<Circle>& circles, Color color) override;

  void draw_fill_circle(float x, float y, float r, Color color) override;
  void draw_string(float x, float y, const std::string& str, Color color) override;
  void draw_string_centered(float x, float y, const std::string& str, Color color) override;

  int get_width() override { return width; }
  int get_height() override { return height; }

  void toggle_fullscreen();

  void clear () override;

  /** Flip the double buffered display */
  void flip () override;

  void enter_fullscreen() override;
  void leave_fullscreen() override;

  /** perform the real flip, only flip marked reagions */
  void real_flip() override;

  void flip (int x1, int y1, int x2, int y2) override;

  // Input Context stuff
  int get_mouse_x () override;
  int get_mouse_y () override;

  bool get_key (int key) override;

  /** Waits for events to come in, blocks until new events are available */
  void wait_for_events_blocking();

  void wait_for_events();

  void run();

  void set_clip_rect (int x1_, int y1_, int x2_, int y2_) override;

  unsigned int get_color_value(const Color& color);
  XColor get_xcolor(const Color& color);

  void set_cursor_real(CursorType cursor) override;
private:
  bool read_event ();
  void send_button_press (int i);
  void send_button_release (int i);
  void send_load_or_save(int n);

  X11Display (const X11Display&);
  X11Display& operator= (const X11Display&);
};

#endif

/* EOF */
