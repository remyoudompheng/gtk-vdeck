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

#ifndef LIST_VIEW_H
#define LIST_VIEW_H

#include <string>
#include <gtkmm.h>

#include <VDeck.hpp>

/** A Gtk:TreeView derived class to display a VDeck library of VCard structures
 */
class ListView : public Gtk::TreeView
{
public:
  /** Standard constructor for derived widget classes
   * @param cobject The GtkTreeView structure
   * @param refBuilder A reference to the Gtk::Builder holding the widget
   */
  ListView(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
  virtual ~ListView();

  /** Fills the Gtk::ListStore list_widget with records
   * @param deck A VDeck structure containing VCards
   */
  void fill_data(VDeck deck);

  std::set<Glib::ustring> filter;
  void update_filter();

protected:
  /** The Gtk::Builder which created the widget
   */
  Glib::RefPtr<Gtk::Builder> uidef;

  class Columns;
  Columns *cols;

  /** The TreeModel containing data.
   */
  Glib::RefPtr<Gtk::ListStore> list_store;
  Glib::RefPtr<Gtk::TreeModelFilter> list_filtered;
  Glib::RefPtr<Gtk::TreeModelSort> list_sorted;

  /** Called when a row is activated
   * @param path Activated row, given by a path
   * @param column Activated column
   */
  void on_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);

  bool _filtered_visibility(Gtk::TreeModel::const_iterator iter);
};

class ListView::Columns : public Gtk::TreeModel::ColumnRecord
{
public:
  Columns() {
    add(fullname); add(familyN); add(firstN);
    add(phone); add(email); add(path); add(vcard);
  }

  /// Full name of the entry
  Gtk::TreeModelColumn<Glib::ustring> fullname;
  /// Name
  Gtk::TreeModelColumn<Glib::ustring> familyN;
  /// 1st name
  Gtk::TreeModelColumn<Glib::ustring> firstN;
  /// Phone number
  Gtk::TreeModelColumn<Glib::ustring> phone;
  /// E-mail address
  Gtk::TreeModelColumn<Glib::ustring> email;
  /// File path
  Gtk::TreeModelColumn<Glib::ustring> path;
  /// Full VCard structure
  Gtk::TreeModelColumn<VCard> vcard;
};

#endif //!LIST_VIEW_H
