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

/** VCard: a class which represents fields for a vCard
 * It should represent faithfully the fields described in RFC2426
 * (version 3.0 of vCard format)
 */
class VCard
{
public:
  VCard() {}
  /** Reads a vCard file into memory
   * @param filename The path of the file to be read
   */
  VCard(const char *filename);
  virtual ~VCard();

  /** Reads a vCard file into memory
   * @param filename The path of the file to be read
   */
  void open(const char* filename);
  
  // Fields from RFC2426 sec. 3.1
  /// Full name of the object
  Glib::ustring fullname;
  /** A semicolon separated structured text:
   * - family name
   * - first name
   * - additional names
   * - honorific prefixes
   * - honorific suffixes
   */
  Glib::ustring name;
  /// Nickname
  Glib::ustring nickname;
  /// A base64 encoded photo
  Glib::ustring photo;
  /// Birth date
  Glib::ustring birthday;
  // Fields from RFC2426 sec. 3.3
  /// Telephone number
  Glib::ustring tel;
  /// E-mail address
  Glib::ustring email;
  /// The mailer software used by the contact
  Glib::ustring mailer;
  // Fields from RFC2426 sec. 3.6
  /// A unique identifier for the contact
  std::string uid;
  /// The version of the vCard (3.0)
  Glib::ustring version;
  /// The iterator type for custom fields
  typedef std::map<std::string, Glib::ustring>::const_iterator const_iterator;
  /// A hash map for custom fields
  std::map<std::string, Glib::ustring> fields;
  /// Full path of the associated vCard file
  std::string filepath;

  bool operator< (const VCard & b) const;
  friend std::ostream& operator<< (std::ostream &out, VCard const & that);

private:
  /** Reads a line of the form field:value
   * @param line The string to be parsed
   */
  void read_field(std::string const line);
  // Print a VCard from the stored fields
  std::string print_me() const;
};

std::ostream& operator<< (std::ostream &out, VCard const & that);

#endif //!VCARD_H
