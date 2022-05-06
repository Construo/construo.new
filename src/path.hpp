// Construo - A wire-frame construction game
// Copyright (C) 2022 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_CONSTRUO_PATH_HPP
#define HEADER_CONSTRUO_PATH_HPP

#include <string>

std::string path_join(std::string const& lhs, std::string const& rhs);
std::string path_dirname(std::string const& path);
std::string path_basename(std::string const& path);

#endif

/* EOF */