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

#ifndef HEADER_CONSTRUO_UNIX_SYSTEM_HPP
#define HEADER_CONSTRUO_UNIX_SYSTEM_HPP

#include <string>
#include "system_context.hpp"

/** This class wraps a bunch of unix functions to provide file-io and
    the current time */
class UnixSystem : public SystemContext
{
private:
  unsigned int start_time;

  /** The location of the users construo directory, aka ~/.construo/,
      the name contains a trailing slash */
  std::string construo_rc_path;

public:
  UnixSystem ();
  virtual ~UnixSystem ();

  unsigned int get_time () override;
  void sleep (unsigned long) override;
  std::string get_construo_rc_path() override;
  std::string get_user_realname() override;
  std::string get_user_email() override;

  FILE* open_input_file(const std::string& filename) override;
  FILE* open_output_file(const std::string& filename) override;

  unsigned int get_mtime (const std::string& filename) override;

  FileType get_file_type(const std::string& filename) override;
  std::vector<std::string> read_directory(const std::string& pathname) override;

  std::string translate_filename(const std::string&) override;
};

#endif

/* EOF */
