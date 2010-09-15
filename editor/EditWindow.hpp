/*
 * math-bibmanager
 * A Bibliography manager for mathematicians
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

#ifndef EDIT_WINDOW_H
#define EDIT_WINDOW_H

#include <string>
#include <gtkmm.h>
#include <VCard.hpp>

class EditWindow : public Gtk::Window
{
public:
  EditWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
  virtual ~EditWindow();
  void set_path(std::string path);

protected:
  /// The parent Gtk::Builder object
  Glib::RefPtr<Gtk::Builder> uidef;

  /// TreeView widget displaying e-mail addresses
  Gtk::TreeView *tree_email;
  Glib::RefPtr<Gtk::ListStore> store_email;
  class EmailColumns;
  EmailColumns *cols_email;

  /// TreeView widget displaying addresses
  Gtk::TreeView *tree_adr;
  Glib::RefPtr<Gtk::ListStore> store_adr;
  class AdrColumns;
  AdrColumns *cols_adr;

  /// Path to the vCard file
  std::string dir_path;

  /// Data structure holding the vCard
  VCard data;

  /** Update text in a TextEntry widget.
   * @name Identifier of a widget in Gtk::Builder uidef
   * @text Text to display
   */
  void set_text(Glib::ustring name, Glib::ustring text);

  /// Displays the current VCard
  void update_display();

  /** Get text from a TextEntry widget.
   * @name Identifier of a widget in Gtk::Builder uidef
    */
  Glib::ustring get_text(Glib::ustring name);

  /// Update the VCard structure with input text
  void update_data();

  // Callbacks
  /// The type of void(void) callbacks
  typedef void (EditWindow::*callback)();
  /** Connects a callback f to an action object from the Gtk::Builder uidef
   * @param name The identifier of the action to connect
   * @param f The callback
   */
  void connect_action(Glib::ustring name, callback f) {
    Glib::RefPtr<Gtk::Action> a = Glib::RefPtr<Gtk::Action>::cast_dynamic(uidef->get_object(name));
    a->signal_activate().connect(sigc::mem_fun(*this, f));
  }

  /// Add an empty address
  void _on_adr_add_activate();
  /// Add an empty e-mail address
  void _on_email_add_activate();
  /// Called upon editing of an e-mail address
  void _on_edited_email_addr(const Glib::ustring& path, const Glib::ustring& new_text);
  /// Called upon editing of an address
  void _on_edited_adr(const Glib::ustring& path, const Glib::ustring& new_text);
  /** Saves the vCard to a file.
   * @see VCard::write_back()
   */
  void _on_save_activate();
  /// Closes the window.
  void _on_close_activate();
};

EditWindow* get_with_builder();

/// Column template for e-mail addresses
class EditWindow::EmailColumns : public Gtk::TreeModelColumnRecord
{
public:
  EmailColumns() {
    add(type); add(adr);
  }
  /// Entry type
  Gtk::TreeModelColumn<Glib::ustring> type; // 0
  /// E-mail address
  Gtk::TreeModelColumn<Glib::ustring> adr;  // 1
};

/// Column template for addresses
class EditWindow::AdrColumns : public Gtk::TreeModelColumnRecord
{
public:
  AdrColumns() {
    add(type); add(text);
  }
  /// Entry type
  Gtk::TreeModelColumn<Glib::ustring> type; // 0
  /// Structured address
  Gtk::TreeModelColumn<Glib::ustring> text;  // 1
};

#endif //!EDIT_WINDOW_H
