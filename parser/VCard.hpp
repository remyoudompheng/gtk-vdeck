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
#include <StructuredText.hpp>

/** VCard: a class which represents fields for a vCard
 * It should represent faithfully the fields described in RFC2426
 * (version 3.0 of vCard format)
 */
class VCard
{
public:
  VCard();
  /** Reads a vCard file into memory
   * @param filename The path of the file to be read
   */
  VCard(const char *filename);
  virtual ~VCard();

  /** A template for storage of multiple fields with a TYPE value.
   */
  template<typename T> class FieldList : public std::vector< std::pair<CommaStruct, T> >
  {
  public:
    FieldList() {}
    ~FieldList() {}

    typedef std::pair<CommaStruct, T> item_t;

    /// Field name
    std::string field;

    /** Appends a value to the list
     * @param type  A strucutred comma-separated list of types
     * @param value The associated value
     */
    void append(CommaStruct type, T value) {
      std::vector<item_t>::push_back(item_t(type, value));
    }

    // friend std::ostream& operator<< (std::ostream &out, VCard::FieldList<T> const & item);
  };

  /** Reads a vCard file into memory
   * @param filename The path of the file to be read
   */
  void open(const char* filename);
  void write_back();

  /// Type of TEL field
  typedef FieldList<Glib::ustring> tel_t;
  /// Type of e-mail field
  typedef FieldList<Glib::ustring> email_t;
  /// Type of ADR field
  typedef FieldList<SemicolonStruct> adr_t;

  // Identification fields (RFC2426 3.1)
  /// Full name of the object
  Glib::ustring fullname;
  /** @brief Structured name of the contact
   *
   * A semicolon separated structured text:
   * - family name
   * - first name
   * - additional names
   * - honorific prefixes
   * - honorific suffixes
   */
  SemicolonStruct name;
  /// Nickname
  Glib::ustring nickname;
  /// A base64 encoded photo
  Glib::ustring photo;
  /// Birth date
  Glib::ustring birthday;

  // Delivery addressing fields (RFC2426 3.2)
  /** @brief Structured mailing address
   *
   * A semicolon separated structured text:
   * - post office box
   * - extended address
   * - street address
   * - locality
   * - region
   * - postal code
   * - country name
   */
  adr_t adr;
  /// Formatted address
  Glib::ustring label;
  // Telecommunications fields (RFC2426 3.3)
  /// Telephone numbers
  tel_t tel;
  /// E-mail address
  email_t email;
  /// Mailer software used by the contact
  Glib::ustring mailer;

  // Geographical fields (RFC2426 3.4)
  /// Time zone
  Glib::ustring tz;
  /// Longitude/latitude of the contact
  Glib::ustring geo;
  // Organizational fields (RFC2426 3.5)
  Glib::ustring title;
  Glib::ustring role;
  Glib::ustring logo;
  Glib::ustring agent;
  Glib::ustring org;
  // Explanatory fields (RFC2426 3.6)
  /// Comma-separated list of categories
  CommaStruct categories;
  /// Supplemental information
  std::string note;
  /// Identification of the product which created the vCard
  std::string prodid;
  /// Revision information
  std::string rev;
  /// Sorting key
  std::string sort_string;
  /// Sound
  std::string sound;
  /// Unique identifier for the contact
  std::string uid;
  /// URL for the object
  std::string url;
  /// Version of the vCard format (3.0)
  Glib::ustring version;

  /// The iterator type for custom fields
  typedef std::map<std::string, Glib::ustring>::const_iterator const_iterator;
  /// A hash map for custom fields
  std::map<std::string, Glib::ustring> fields;
  /** @brief Full path of the associated vCard file.
   *
   * The string filepath is supposed to refer to an existing
   * file which will be overwritten upon calling write_back.
   * @see write_back()
   */
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


#endif //!VCARD_H
