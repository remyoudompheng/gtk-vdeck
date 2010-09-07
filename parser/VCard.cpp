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
#include <sstream>

using namespace std;

void VCard::read_field(const string line)
{
  size_t t = line.find_first_of(":");
  if(t == string::npos) return;

  if (line.length() >= t) {
    string field, content;
    field = line.substr(0, t);
    if (field == "BEGIN") return;
    if (field == "END") return;
    content = line.substr(t+1);
    fields[field] = content;
  }
  else {
    cout << "Invalid line read: " << line << endl;
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

ostream& operator<< (ostream &out, VCard const & that)
{
  VCard::const_iterator iter;
  for (iter = that.fields.begin();
       iter != that.fields.end();
       iter++)
    {
      out << iter->first << ":" << iter->second << endl;
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
  catch(const out_of_range & ex) {
    cout << "Out of range exception: " << ex.what() << endl;
  }
}

