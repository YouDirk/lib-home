/* lib@home, framework to develop distributed calculations.
 * Copyright (C) 2020  Dirk "YouDirk" Lehmann
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "libathome-common/File.hpp"
#include "libathome-common/Error.hpp"


#ifndef OSWIN
const char* libathome_common::File::PATH_SEPERATOR = "/";
const char* libathome_common::File::PATH_DOT = ".";
const char* libathome_common::File::PATH_DOTDOT = "..";
#else /* ifndef OSWIN  */
const char* libathome_common::File::PATH_SEPERATOR = "\\";
const char* libathome_common::File::PATH_DOT = ".";
const char* libathome_common::File::PATH_DOTDOT = "..";
#endif /* ifndef OSWIN  */


libathome_common::File::
File(FILE* fstream, const std::string& stream_name) noexcept(false)
  :extern_fstream(fstream), binary(true), path("<no path>"),
   filename("<extern fstream>"), fstream(NULL),
   mode(File::access_t::read_e)
{
  if (fstream == NULL) {
    throw
      Err("Argument FSTREAM '%s' was set to NULL!", stream_name.c_str());
  }

  filename_full = stream_name;
}

libathome_common::File::
File(const std::string& path, const std::string& filename, bool binary)
  :extern_fstream(NULL), binary(binary), path(path), filename(filename),
   fstream(NULL), mode(File::access_t::read_e)
{
  filename_full = this->path + File::PATH_SEPERATOR + this->filename;
}

libathome_common::File::
~File()
{
  this->close();
}

void libathome_common::File::
open(File::access_t mode)
{
  this->mode = mode;

  if (this->extern_fstream != NULL) {
    this->fstream = this->extern_fstream;
    return;
  }

  
}

void libathome_common::File::
close()
{
  this->mode = File::access_t::read_e;

  if (this->extern_fstream != NULL) {
    this->fstream = NULL;
    return;
  }

  
  this->fstream = NULL;
}

void libathome_common::File::
vprintf(const std::string& fmt, va_list ap) const
{
  if (this->fstream == NULL
      || (this->mode != File::access_t::write_e &&
          this->mode != File::access_t::append_e)) {
    throw Err("File '%s' not opened for write- or append-access!",
              this->filename_full.c_str());
  }

  if (0 >= vfprintf(this->fstream, fmt.c_str(), ap))
    throw Err("Could not write to '%s'!", this->filename_full.c_str());
}
