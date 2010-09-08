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

#ifndef STRUCTURED_TEXT_H
#define STRUCTURED_TEXT_H

#include <glibmm.h>

class StructuredText : public std::list<Glib::ustring>
{
public:
  StructuredText() {}
  StructuredText(Glib::ustring s) { read_str(s); }
private:
  Glib::ustring buf;
protected:
  std::string delimiter;
  void read_str(Glib::ustring s);
};

class CommaStruct : public StructuredText
{
public:
  CommaStruct() { delimiter = ","; }
  CommaStruct(Glib::ustring s) { delimiter = ","; read_str(s); }
};

class SemicolonStruct : public StructuredText
{
public:
  SemicolonStruct() { delimiter = ";"; }
  SemicolonStruct(Glib::ustring s) { delimiter = ";"; read_str(s); }
};

#endif //!VDECK_H
