/*
 * gtk-vcard
 * A viewer and editor for collections of VCards
 *
 * Copyright (C) 2010 Rémy Oudompheng

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "StructuredText.hpp"

void StructuredText::read_str(Glib::ustring s)
{
  // Find first comma
  size_t n = s.find_first_of(delimiter);
  // No comma ?
  if(n == Glib::ustring::npos) {
    push_back(buf + s);
    buf.clear();
    return;
  }
  // Escaped comma
  if ((n != 0) && (s[n-1] == '\\')) {
    buf += s.substr(0,n-1);
    buf += delimiter;
    read_str(s.substr(n+1));
    return;
  }
  // Normal comma
  push_back(buf + s.substr(0,n));
  buf.clear();
  read_str(s.substr(n+1));
  return;
}
