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

using namespace std;

void StructuredText::read_str(Glib::ustring s)
{
  clear();
  Glib::ustring buf, tmp(s);

  size_t n = tmp.find_first_of(delimiter);
  while(n != Glib::ustring::npos) {
    if ((n != 0) && (tmp[n-1] == '\\')) {
      // Escaped delimiter
      buf += tmp.substr(0,n-1);
      buf += delimiter;  
    } else {
      // Normal delimiter
      push_back(buf + tmp.substr(0,n));
      buf.clear();
    }
    tmp = tmp.substr(n+1);
    n = tmp.find_first_of(delimiter);
  }
  push_back(buf + tmp);
}

std::string StructuredText::join() const
{
  std::string r;
  for(StructuredText::const_iterator i = begin();
      i != end(); i++)
    {
      if(i != begin()) r += delimiter;
      string s(*i);
      size_t n = s.find_first_of(delimiter);
      while(n != string::npos) {
	r += s.substr(0,n) + "\\" + delimiter;
	s = s.substr(n+1);
	n = s.find_first_of(delimiter);
      }
      r += s;
    }
  return r;
}

ostream& operator<<(ostream& out, StructuredText const & that)
{
  out << that.join();
  return out;
}
