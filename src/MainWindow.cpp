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

#include "MainWindow.hpp"

#include <iostream>

using namespace std;

typedef Glib::RefPtr<Gtk::TreeViewColumn> ColPtr;

MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
  : Gtk::Window(cobject),
    uidef(refBuilder)
{
  // Actions
  connect_action("act_openlib", &MainWindow::_on_openlib_activate);
  connect_action("act_refresh", &MainWindow::_on_refresh_activate);
  connect_action("act_add", &MainWindow::_on_add_activate);
  connect_action("act_quit", &MainWindow::_on_quit_activate);

  // List view
  refBuilder->get_widget_derived("tree_main", list_view);

  // Category list
  cat_cols = new CatColumns();
  cat_store = Gtk::ListStore::create(*cat_cols);
  refBuilder->get_widget("tree_cats", cat_view);
  cat_view->set_model(cat_store);

  ColPtr treecol_cats = ColPtr::cast_static(refBuilder->get_object("treecol_cats"));
  treecol_cats->set_sort_column(cat_cols->name);

  // Directory list
  dir_store = Gtk::ListStore::create(*cat_cols);
  refBuilder->get_widget("tree_dirs", dir_view);
  dir_view->set_model(dir_store);

  ColPtr treecol_dirs = ColPtr::cast_static(refBuilder->get_object("treecol_dirs"));
  treecol_dirs->set_sort_column(cat_cols->name);

  // Handle filtering
  cat_view->get_selection()->set_mode(Gtk::SELECTION_MULTIPLE);
  cat_view->get_selection()->signal_changed().
    connect( sigc::mem_fun(*this, &MainWindow::_on_catselection_changed) );
  dir_view->get_selection()->set_mode(Gtk::SELECTION_MULTIPLE);
  dir_view->get_selection()->signal_changed().
    connect( sigc::mem_fun(*this, &MainWindow::_on_dirselection_changed) );
}

MainWindow::~MainWindow()
{
}

/** Open the specified directory in the list display.
 * @param path Path to the directory to display.
 */
void MainWindow::set_path(string path)
{
  dir_path = path;
  update_library();
}

void MainWindow::update_library()
{
  directory.clear();
  directory.import_dir(dir_path);
  list_view->fill_data(directory);
  update_cats();
  update_dirs();
}

void MainWindow::update_cats()
{
  // Fill category list
  set<Glib::ustring> categories;
  for(VDeck::iterator i = directory.begin();
      i != directory.end(); i++) {
    for(CommaStruct::const_iterator c = i->categories.begin();
	c != i->categories.end(); c++) {
      categories.insert(*c);
    }
  }
  // Fill ListStore
  cat_store->clear();
  Gtk::TreeIter i = cat_store->append();
  (*i)[cat_cols->name] = "[none]";
  for(set<Glib::ustring>::iterator c = categories.begin();
      c != categories.end(); c++) {
    Gtk::TreeIter i = cat_store->append();
    (*i)[cat_cols->name] = *c;
  }
  // Select all categories by default
  cat_view->get_selection()->select_all();
}

void MainWindow::update_dirs()
{
  // Fill folder list
  set<Glib::ustring> dirs;
  for(VDeck::iterator i = directory.begin();
      i != directory.end(); i++)
    dirs.insert(i->reldir);
  // Fill ListStore
  dir_store->clear();
  for(set<Glib::ustring>::iterator d = dirs.begin();
      d != dirs.end(); d++) {
    Gtk::TreeIter i = dir_store->append();
    (*i)[cat_cols->name] = *d;
  }
  // Use all folders by default
  dir_view->get_selection()->select_all();
}

/// Updates filter for the ListView widget
void MainWindow::_on_catselection_changed()
{
  typedef Gtk::TreeView::Selection::ListHandle_Path pathlist;
  pathlist sel = cat_view->get_selection()->get_selected_rows();
  list_view->filter.clear();
  for(pathlist::const_iterator i = sel.begin();
      i != sel.end(); i++)
    {
      Gtk::TreeIter it = cat_store->get_iter(*i);
      list_view->filter.insert((*it)[cat_cols->name]);
    }

  list_view->update_filter();
}

void MainWindow::_on_dirselection_changed()
{
  typedef Gtk::TreeView::Selection::ListHandle_Path pathlist;
  pathlist sel = dir_view->get_selection()->get_selected_rows();
  list_view->filter_dir.clear();
  for(pathlist::const_iterator i = sel.begin();
      i != sel.end(); i++)
    {
      Gtk::TreeIter it = dir_store->get_iter(*i);
      list_view->filter_dir.insert((*it)[cat_cols->name]);
    }

  list_view->update_filter();
}

/// Creates a new vCard file and adds it to the current VDeck
void MainWindow::_on_add_activate()
{
  Gtk::FileChooserDialog dialog("Choose a filename to create",
                                Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_current_folder(dir_path);
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::NEW, Gtk::RESPONSE_OK);

  int result = dialog.run();
  if (result != Gtk::RESPONSE_OK) return;

  // Fill vCard view
  Glib::ustring path = dialog.get_filename();
  directory.create_new(path);
  list_view->fill_data(directory);
  update_cats();
}

/// Open a folder
void MainWindow::_on_openlib_activate()
{
  Gtk::FileChooserDialog dialog("Choose the path of the library ",
                                Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

  int result = dialog.run();
  if (result != Gtk::RESPONSE_OK) return;

  set_path(dialog.get_filename());
}

/// Refresh the library
void MainWindow::_on_refresh_activate()
{
  update_library();
}

/// Quits the program
void MainWindow::_on_quit_activate()
{
  Gtk::Main::quit();
}
