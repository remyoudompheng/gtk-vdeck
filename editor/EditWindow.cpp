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

#include "EditWindow.hpp"
#include <editor_xml.h>
#include <StructuredText.hpp>
#include <iostream>

using namespace std;

EditWindow* get_with_builder()
{
  Glib::ustring uidef(editor_xml, editor_xml_len);
  Glib::RefPtr<Gtk::Builder> refGlade = Gtk::Builder::create();
  bool ok = refGlade->add_from_string(uidef);

  EditWindow *main_win = 0;
  refGlade->get_widget_derived("editor_win", main_win);
  return main_win;
}


EditWindow::EditWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
  : Gtk::Window(cobject),
    uidef(refBuilder)
{
  // Actions
  connect_action("act_close", &EditWindow::_on_close_activate);
}

EditWindow::~EditWindow()
{
}

void EditWindow::set_text(Glib::ustring name, Glib::ustring text)
{
  Gtk::Entry *widget;
  uidef->get_widget(name, widget);
  widget->set_text(text);
}

void EditWindow::update_display()
{
  set_text("entry_fullname", data.fullname);
  SemicolonStruct name(data.name);
  if (name.size() >= 5) {
    set_text("entry_familyN", name[0]);
    set_text("entry_firstN", name[1]);
    set_text("entry_addN", name[2]);
    set_text("entry_prefix", name[3]);
    set_text("entry_suffix", name[4]);
  } else {
    cerr << "Invalid N field : " << data.name << endl;
  }
}

/** Open the specified file
 * @param path Path to the file.
 */
void EditWindow::set_path(string path)
{
  dir_path = path;
  data.open(path.c_str());
  cerr << "Read data:" << endl << data;
  update_display();
}

void EditWindow::_on_close_activate()
{
  hide_all();
}
