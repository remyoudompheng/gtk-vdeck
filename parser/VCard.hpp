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

#ifndef VCARD_H
#define VCARD_H

#include <string>
#include <map>
#include <glibmm.h>

class VCard
{
public:
  VCard(const char *filename);
  virtual ~VCard();

  typedef std::map<std::string, std::string>::const_iterator const_iterator;

  std::map<std::string, std::string> fields;
  // Path of the associated document
  std::string filepath;

  bool operator< (const VCard & b) const;

  friend std::ostream& operator<< (std::ostream &out, VCard const & that);

private:
  void read_field(std::string const line);
};

std::ostream& operator<< (std::ostream &out, VCard const & that);

#endif //!VCARD_H
