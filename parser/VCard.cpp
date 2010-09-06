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

#include "VCard.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
// #include <sstream>
// #include <sys/stat.h>

using namespace std;

void VCard::read_field(const string line)
{
  size_t t = line.find_first_of(":");
  string field, content;
  if (line.length() >= t) {
    string field, content;
    field = line.substr(0, t-1);
    if (field == "BEGIN") return;
    if (field == "END") return;
    content = line.substr(t);
    fields[field] = content;
  }
  else {
    cout << "Invalid line read: " << line;
  }
}

VCard::VCard(const char* filename)
{
  // Initialise fields
  filepath = filename;

  ifstream source;
  source.open(filename, ifstream::in);


  string l;
  while(source.good()) {
    getline(source, l);
    read_field(l);
  }
}

VCard::~VCard() {}

void VCard::print_me() const
{
  const_iterator iter;
  for (iter = fields.begin();
       iter != fields.end();
       iter++)
    {
      cout << iter->first << ":" << iter->second << endl;
    }
}

bool VCard::operator<(const VCard & b) const
{
  try {
    if (fields.at("UID") < b.fields.at("UID"))
      {
	return true;
      }
  }
  catch(const std::out_of_range & ex) {
    cout << "Out of range exception: " << ex.what() << endl;
  }
}

