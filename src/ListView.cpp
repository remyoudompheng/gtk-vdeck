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
#include <EditWindow.hpp>

using namespace std;

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

  // Filters for the bibliography list
  list_filtered = Gtk::TreeModelFilter::create(list_widget);
  list_filtered->set_visible_func( sigc::mem_fun(*this, &ListView::_filtered_visibility) );
  set_model(Gtk::TreeModelSort::create(list_filtered));
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
      if(i->email.size())
	(*iter)[cols->email] = i->email[0].second;
      (*iter)[cols->path] = i->filepath;
      (*iter)[cols->vcard] = *i;
  }
}

void ListView::update_filter()
{
  list_filtered->refilter();
}

bool ListView::_filtered_visibility(Gtk::TreeModel::const_iterator iter)
{
  VCard item = (*iter)[cols->vcard];
  CommaStruct cats = item.categories;
  if(cats.empty()) { return filter.find("[none]") != filter.end(); }
  for(CommaStruct::const_iterator i = cats.begin();
      i != cats.end(); i++)
    {
      if(filter.find(*i) != filter.end()) return true;
    }
  return false;
}

void ListView::on_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
{
  Gtk::TreeIter i = list_widget->get_iter(path);
  VCard v = (*i)[cols->vcard];

  EditWindow *win = get_with_builder();
  win->set_path(v.filepath);
  win->show_all();
}
