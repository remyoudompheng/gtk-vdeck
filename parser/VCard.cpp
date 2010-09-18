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

VCard::VCard()
  : version("3.0")
{
  name.resize(5);
  tel.field = "TEL";
  adr.field = "ADR";
  email.field = "EMAIL";
}

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

  // Parse %field:%type
  t = field.find_first_of(";");
  CommaStruct type;
  if (t != string::npos) {
    if ( ( field.substr(t+1).compare(0,5, "TYPE=") == 0 )
	 || ( field.substr(t+1).compare(0,5, "type=") == 0 ) )
      type.read_str(field.substr(t+6));
    field = field.substr(0,t);
#ifdef DEBUG
    cerr << "Parsed type = " << type << " for field " << field;
#endif
  }

  // TODO: fully implement RFC2426 (VCard 3.0)
  field = field.uppercase();
  // exceptional fields
  if((field == "BEGIN") || (field == "END")) {
    if (content.uppercase() != "VCARD")
      cerr << "VCard " << filepath << ": BEGIN and END must contain 'VCARD'" << endl;
    return;
  }
  // sec 3.1: identification types
  // FN, N, NICKNAME, PHOTO, BDAY
  if (field == "FN") { fullname = content; return; }
  if (field == "N") { name.read_str(content); return; }
  if (field == "NICKNAME") { nickname = content; return; }
  if (field == "PHOTO") { photo = content; return; }
  if (field == "BDAY") { birthday = content; return; }
  // sec 3.2: delivery addr. types (TODO);
  // ADR, LABEL
  if (field == "ADR") { adr.append(type, SemicolonStruct(content)); return; }
  if (field.compare(0, 5, "LABEL") == 0) { label = content; return; }
  // sec 3.3: telecom addr. types;
  // TEL, EMAIL, MAILER
  if(field == "TEL") { tel.append(type, content); return; }
  if(field == "EMAIL") { email.append(type,content); return; }
  if (field == "MAILER" ) { mailer = content; return; }
  // sec 3.4: geographical types
  // TZ, GEO
  if (field == "TZ" ) { tz = content; return; }
  if (field == "GEO" ) { geo = content; return; }
  // sec 3.5: organizational types
  // TITLE, ROLE, LOGO, AGENT, ORG
  if (field == "TITLE" ) { title = content; return; }
  if (field == "ROLE" ) { role = content; return; }
  if (field == "LOGO" ) { logo = content; return; }
  if (field == "AGENT" ) { agent = content; return; }
  if (field == "ORG" ) { org = content; return; }
  // sec 3.6: explanatory types
  // CATEGORIES, NOTE, PRODID, REV, SORT-STRING, SOUND, UID, URL, VERSION
  if (field == "CATEGORIES" ) { categories.read_str(content); return; }
  if (field == "NOTE" ) { note = content; return; }
  if (field == "PRODID" ) { prodid = content; return; }
  if (field == "REV" ) { rev = content; return; }
  if (field == "SORT-STRING" ) { sort_string = content; return; }
  if (field == "SOUND" ) { sound = content; return; }
  if (field == "UID" ) { uid = content; return; }
  if (field == "URL" ) { url = content; return; }
  if (field == "VERSION" ) { version = content; return; }

  // other types
  fields[field] = content;
  return;
}

VCard::VCard(const char* filename)
{
  open(filename);
}

void VCard::open(const char* filename)
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

/// Writes the VCard back to a file
void VCard::write_back()
{
  ofstream out;
  out.open(filepath.c_str(), ofstream::out);
  out << *this;
}

template<class T>
ostream& operator<< (ostream &out, VCard::FieldList<T> const & item)
{
  typename VCard::FieldList<T>::const_iterator i;
  for(i = item.begin(); i != item.end(); i++)
    out << item.field << ";" << "TYPE=" << i->first << ":" << i->second << endl;
  return out;
}

/** Formats data from a vCard structure to a vCard file
 * @param out Output stream
 * @param that The structure to print
 */
ostream& operator<< (ostream &out, VCard const & that)
{
  out << that.print_me() << endl;
  return out;
}

string VCard::print_me() const {
  ostringstream out;
  out << "BEGIN:VCARD" << endl;
  // Mandatory fields
  out << "VERSION:" << "3.0" << endl;
  out << "UID:" << uid << endl;
  out << "FN:" << fullname << endl;
  out << "N:" << name << endl;
  // standard fields
  // section 3.1
  if(nickname.length()) out << "NICKNAME:" << nickname << endl;
  if(photo.length()) out << "PHOTO:" << photo << endl;
  if(birthday.length()) out << "BDAY:" << birthday << endl;
  // section 3.2
  out << adr;
  if(label.length()) out << "LABEL:" << label << endl;
  // section 3.3
  out << tel;
  out << email;
  if(mailer.length()) out << "MAILER:" << mailer << endl;
  // section 3.4
  if(tz.length()) out << "TZ:" << tz << endl;
  if(geo.length()) out << "GEO:" << geo << endl;
  // section 3.5
  if(title.length()) out << "TITLE:" << title << endl;
  if(role.length()) out << "ROLE:" << role << endl;
  if(logo.length()) out << "LOGO:" << logo << endl;
  if(agent.length()) out << "AGENT:" << agent << endl;
  if(org.length()) out << "ORG:" << org << endl;
  // section 3.6
  if(categories.size()) out << "CATEGORIES:" << categories << endl;
  if(note.length()) out << "NOTE:" << note << endl;
  if(prodid.length()) out << "PRODID:" << prodid << endl;
  if(rev.length()) out << "REV:" << rev << endl;
  if(sort_string.length()) out << "SORT-STRING:" << sort_string << endl;
  if(sound.length()) out << "SOUND:" << sound << endl;
  if(url.length()) out << "URL:" << url << endl;

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

/** Comparison operator:
 * implementation compares UIDs of vCards.
 * @param b another vCard
 */
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

  return false;
}
