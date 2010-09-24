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
#include <iostream>
#include <cassert>

using namespace std;

ListView::ListView(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
  : Gtk::TreeView(cobject),
    uidef(refBuilder)
{
  cols = new Columns();
  list_store = Gtk::ListStore::create(*cols);
  list_filtered = Gtk::TreeModelFilter::create(list_store);
  list_sorted = Gtk::TreeModelSort::create(list_filtered);

  // Sort by last name by default
  list_sorted->set_sort_column(cols->familyN, Gtk::SORT_ASCENDING);

  // Filters for the bibliography list
  list_filtered->set_visible_func( sigc::mem_fun(*this, &ListView::_filtered_visibility) );
  set_model(list_sorted);
}

ListView::~ListView()
{
}

void ListView::fill_data(VDeck deck)
{
  list_store->clear();
  VDeck::iterator i;
  for(i = deck.begin(); i != deck.end(); i++)
  {
      Gtk::TreeIter iter;
      iter = list_store->append();
      (*iter)[cols->fullname] = i->fullname;
      (*iter)[cols->familyN] = i->name[0];
      (*iter)[cols->firstN] = i->name[1];
      if(i->tel.size())
	(*iter)[cols->phone] = i->tel[0].second;
      if(i->email.size())
	(*iter)[cols->email] = i->email[0].second;
      (*iter)[cols->path] = i->relpath;
      (*iter)[cols->vcard] = *i;
  }
}

void ListView::update_filter()
{
  list_filtered->refilter();
}

bool ListView::_filtered_visibility(Gtk::TreeModel::const_iterator iter)
{
  bool ok = false;
  VCard item = (*iter)[cols->vcard];
  // Check for categories
  CommaStruct cats = item.categories;
  if(cats.empty()) ok = filter.find("[none]") != filter.end();
  for(CommaStruct::const_iterator i = cats.begin();
      i != cats.end(); i++)
    {
      if(filter.find(*i) != filter.end()) ok = true;
    }
  // Check for dir
  return ok &&  filter_dir.find(item.reldir) != filter_dir.end();
}

void ListView::on_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
{
  Gtk::TreeIter i = list_sorted->get_iter(path);
  VCard v = (*i)[cols->vcard];

  EditWindow *win = get_with_builder();
  Gtk::Window *main = NULL;
  uidef->get_widget("main_win", main);
  win->set_transient_for(*main);
  win->set_path(v.filepath);
  win->show_all();
}
