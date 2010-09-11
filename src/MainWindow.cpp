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
using namespace std;

MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
  : Gtk::Window(cobject),
    uidef(refBuilder)
{
  // Actions
  connect_action("act_add", &MainWindow::_on_add_activate);
  connect_action("act_quit", &MainWindow::_on_quit_activate);

  // List view
  refBuilder->get_widget_derived("tree_main", list_view);
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
  directory.import_dir(dir_path);
  list_view->fill_data(directory);
}

/// Creates a new vCard file and adds it to the current VDeck
void MainWindow::_on_add_activate()
{
  Gtk::FileChooserDialog dialog("Choose a library folder",
                                Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::NEW, Gtk::RESPONSE_OK);

  int result = dialog.run();
  if (result != Gtk::RESPONSE_OK) return;
  
  Glib::ustring path = dialog.get_filename();
  directory.create_new(path);
  list_view->fill_data(directory);
}

/// Quits the program
void MainWindow::_on_quit_activate()
{
  Gtk::Main::quit();
}
