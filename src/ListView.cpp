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

#include "ListView.hpp"

ListView::ListView(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
  : Gtk::TreeView(cobject),
    uidef(refBuilder)
{
  cols = new Columns();
  list_widget = Gtk::ListStore::create(*cols);

  Glib::RefPtr<Gtk::TreeViewColumn> treecol_name, treecol_email;
  treecol_name = Glib::RefPtr<Gtk::TreeViewColumn>::cast_static(refBuilder->get_object("treecol_name"));
  treecol_email = Glib::RefPtr<Gtk::TreeViewColumn>::cast_static(refBuilder->get_object("treecol_email"));

  treecol_name->set_sort_column(cols->fullname);
  treecol_email->set_sort_column(cols->email);
  set_model(list_widget);
}

ListView::~ListView()
{
}

void ListView::fill_data(VDeck deck)
{
  list_widget->clear();
  VDeck::iterator i;
  for(i = deck.begin(); i != deck.end(); i++)
  {
      Gtk::TreeIter iter;
      iter = list_widget->append();
      (*iter)[cols->fullname] = i->fullname;
      (*iter)[cols->email] = i->email;
      (*iter)[cols->vcard] = *i;
  }
}
