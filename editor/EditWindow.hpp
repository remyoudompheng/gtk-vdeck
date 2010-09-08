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
  /// Path to the vCard file
  std::string dir_path;
  /// Data structure holding the vCard
  VCard data;

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

  /// Closes the window.
  void _on_close_activate();
};

#endif //!EDIT_WINDOW_H
