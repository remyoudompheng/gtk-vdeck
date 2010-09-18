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
#include <ostream>

/** Class for handling strings with comma or semicolon separated values
 */
class StructuredText : public std::vector<Glib::ustring>
{
public:
  StructuredText() {}
  /** StructuredText constructor
   * @param s The string to be parsed
   */
  StructuredText(Glib::ustring s) { read_str(s); }
  friend std::ostream& operator<<(std::ostream& out, StructuredText const & that);

  /** The main parsing function
   * @param s The string to be parsed
   */
  void read_str(Glib::ustring s);
  std::string join() const;
protected:
  /// The delimiter character
  std::string delimiter;
};

/** Parser for comma separated values
 */
class CommaStruct : public StructuredText
{
public:
  CommaStruct() { delimiter = ","; }
  /** Constructor
   * @param s The string to be parsed
   */
  CommaStruct(Glib::ustring s) { delimiter = ","; read_str(s); }
};

/** Parser for semicolon separated values
 */
class SemicolonStruct : public StructuredText
{
public:
  SemicolonStruct() { delimiter = ";"; }
  /** Constructor
   * @param s The string to be parsed
   */
  SemicolonStruct(Glib::ustring s) { delimiter = ";"; read_str(s); }
};

#endif //!VDECK_H
