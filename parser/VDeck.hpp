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

#ifndef VDECK_H
#define VDECK_H

#include "VCard.hpp"
#include <string>
#include <set>

class VDeck : public std::set<VCard>
{
public:
  VDeck() {}
  VDeck(const char* path) { import_dir(path); }
  friend std::ostream& operator<< (std::ostream &out, VDeck const & that);
  void import_dir(std::string path);
};
std::ostream& operator<< (std::ostream &out, VDeck const & that);

#endif //!VDECK_H
