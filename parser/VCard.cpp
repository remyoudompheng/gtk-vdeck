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

  // Parse "%field:%content"
  Glib::ustring field, content;
  if (line.length() >= t) {
    try {
      field = line.substr(0, t);
      content = line.substr(t+1);
    }
    catch(Glib::ConvertError & ex) {
      cerr << "Conversion error: " << ex.what() << endl;
    }
  }
  else {
    cerr << "Invalid line read: " << line << endl;
    return;
  }

  // TODO: fully implement RFC2426
  field = field.uppercase();
  // exceptional fields
  if((field == "BEGIN") || (field == "END")) {
    if (content.uppercase() != "VCARD")
      cerr << "VCard " << filepath << ": BEGIN and END must contain 'VCARD'" << endl;
    return;
  }
  // sec 3.1: identification types
  if (field == "FN") { fullname = content; return; }
  if (field == "N") { name = content; return; }
  if (field == "NICKNAME") { nickname = content; return; }
  if (field == "PHOTO") { photo = content; return; }
  if (field == "BDAY") { birthday = content; return; }
   
  // sec 3.2: delivery addr. types (TODO);
  // sec 3.3: telecom addr. types;
  if(field.compare(0, 3, "TEL") == 0) {
    tel = content; return;
  }
  if(field.compare(0, 5, "EMAIL") == 0) {
    email = content; return;
  }
  if (field == "MAILER" ) { mailer = content; return; }
  // sec 3.6: explanatory types;
  if (field == "UID" ) { uid = content; return; }
  if (field == "VERSION" ) { version = content; return; }

  // other types
  fields[field] = content;
  return;
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
  out << that.print_me() << endl;
}

string VCard::print_me() const {
  ostringstream out;
  out << "BEGIN:VCARD" << endl;
  // Mandatory fields
  out << "VERSION:" << version << endl;
  out << "FN:" << fullname << endl;
  out << "N:" << name << endl;
  // standard fields
  // section 3.1
  if(nickname.length()) out << "NICKNAME:" << nickname << endl;
  if(photo.length()) out << "PHOTO:" << photo << endl;
  if(birthday.length()) out << "BDAY:" << birthday << endl;
  // section 3.3
  if(tel.length()) out << "TEL:" << tel << endl;
  if(email.length()) out << "EMAIL:" << email << endl;
  if(mailer.length()) out << "MAILER:" << mailer << endl;
  // other fields
  VCard::const_iterator iter;
  for (iter = fields.begin();
       iter != fields.end();
       iter++)
    {
      out << iter->first << ":" << iter->second << endl;
    }
  out << "END:VCARD" << endl;
  return out.str();
}

bool VCard::operator<(const VCard & b) const
{
  try {
    if (uid < b.uid)
      {
	return true;
      }
  }
  catch(const out_of_range & ex) {
    cout << "Out of range exception: " << ex.what() << endl;
  }
}

