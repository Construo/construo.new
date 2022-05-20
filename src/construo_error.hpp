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

#ifndef HEADER_CONSTRUO_ERROR_HPP
#define HEADER_CONSTRUO_ERROR_HPP

#include <iostream>
#include <stdexcept>
#include <string>

#ifndef NODEBUG
#  define ConstruoAssert(expr, str) assert(expr && str)
#endif

namespace construo {

class ConstruoError : public std::exception
{
public:
  std::string msg;

  ConstruoError(const std::string& str) : msg(str) {}

  char const* what() const noexcept override { return msg.c_str(); }

  static void raise(const std::string& str) {
    throw ConstruoError (str);
  }
};

inline
void print_exception(std::exception const& err, int level = 0)
{
  std::cerr << std::string(level, ' ') << "exception: " << err.what() << '\n';

  try {
    std::rethrow_if_nested(err);
  } catch(std::exception const& new_err) {
    print_exception(new_err, level + 1);
  } catch(...) {
    std::cerr << "unknown exception\n";
  }
}

} // namespace construo

#endif

/* EOF */
