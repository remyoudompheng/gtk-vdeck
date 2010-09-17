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
#include <cassert>

using namespace std;

EditWindow* get_with_builder()
{
  Glib::ustring uidef(editor_xml, editor_xml_len);
  Glib::RefPtr<Gtk::Builder> refGlade = Gtk::Builder::create();
  refGlade->add_from_string(uidef);

  EditWindow *main_win = 0;
  refGlade->get_widget_derived("editor_win", main_win);
  return main_win;
}


EditWindow::EditWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
  : Gtk::Window(cobject),
    uidef(refBuilder)
{
  // Address TreeView
  uidef->get_widget("tree_adr", tree_adr);
  cols_adr = new AdrColumns;
  store_adr = Gtk::ListStore::create(*cols_adr);
  tree_adr->set_model(store_adr);

  // Editable cells
  Glib::RefPtr<Gtk::CellRendererText> cell_addr;
  cell_addr = Glib::RefPtr<Gtk::CellRendererText>::cast_static
    (uidef->get_object("treecell_a_text"));
  cell_addr->set_property("editable", true);
  cell_addr->signal_edited().connect(sigc::mem_fun(*this, &EditWindow::_on_edited_adr));

  // Email TreeView
  uidef->get_widget("tree_email", tree_email);
  cols_email = new EmailColumns;
  store_email = Gtk::ListStore::create(*cols_email);
  tree_email->set_model(store_email);

  // Editable cells
  Glib::RefPtr<Gtk::CellRendererText> cell_email;
  cell_email = Glib::RefPtr<Gtk::CellRendererText>::cast_static
    (uidef->get_object("treecell_email"));
  cell_email->set_property("editable", true);
  cell_email->signal_edited().connect(sigc::mem_fun(*this, &EditWindow::_on_edited_email_addr));

  // Actions
  connect_action("act_email_add", &EditWindow::_on_email_add_activate);
  connect_action("act_save", &EditWindow::_on_save_activate);
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
  // Page 1
  set_text("entry_fullname", data.fullname);
  if (data.name.size() >= 5) {
    set_text("entry_familyN", data.name[0]);
    set_text("entry_firstN", data.name[1]);
    set_text("entry_addN", data.name[2]);
    set_text("entry_prefix", data.name[3]);
    set_text("entry_suffix", data.name[4]);
  } else {
    cerr << "Invalid N field : " << data.name << " of size " << data.name.size() << endl;
  }
  set_text("entry_nickname", data.nickname);
  set_text("entry_uid", data.uid);
  set_text("entry_cats", data.categories.join());
  set_text("entry_bday", data.birthday);
  // Page 2
  store_adr->clear();
  for(VCard::adr_t::iterator i = data.adr.begin();
      i != data.adr.end(); i++) {
      Gtk::TreeIter iter = store_adr->append();
      (*iter)[cols_adr->type] = "";
      (*iter)[cols_adr->text] = i->join();
  }
  store_email->clear();
  for(VCard::email_t::iterator i = data.email.begin();
      i != data.email.end(); i++) {
      Gtk::TreeIter iter = store_email->append();
      (*iter)[cols_email->type] = "";
      (*iter)[cols_email->adr] = *i;
  }
  set_text("entry_url", data.url);
}

Glib::ustring EditWindow::get_text(Glib::ustring name)
{
  Gtk::Entry *widget;
  uidef->get_widget(name, widget);
  return widget->get_text();
}

void EditWindow::update_data()
{
  // Page 1
  data.fullname = get_text("entry_fullname");
  data.name.resize(5);
  data.name[0] = get_text("entry_familyN");
  data.name[1] = get_text("entry_firstN");
  data.name[2] = get_text("entry_addN");
  data.name[3] = get_text("entry_prefix");
  data.name[4] = get_text("entry_suffix");
  data.nickname = get_text("entry_nickname");
  data.uid = get_text("entry_uid");
  data.categories.read_str(get_text("entry_cats"));
  data.birthday = get_text("entry_bday");
  // Page 2
  data.email.clear();
  for(Gtk::TreeIter i = store_email->children().begin();
      i != store_email->children().end(); i++) {
    data.email.push_back((*i)[cols_email->adr]);
  }
  data.url = get_text("entry_url");
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

void EditWindow::_on_email_add_activate()
{
  store_email->append();
}

void EditWindow::_on_adr_add_activate()
{
  Gtk::TreeIter i = store_adr->append();
  (*i)[cols_adr->text] = ";;;;;;";
}

void EditWindow::_on_edited_email_addr(const Glib::ustring& path, const Glib::ustring& new_text)
{
  Gtk::TreeIter i = store_email->get_iter(path);
  (*i)[cols_email->adr] = new_text;
}

void EditWindow::_on_edited_adr(const Glib::ustring& path, const Glib::ustring& new_text)
{
  Gtk::TreeIter i = store_adr->get_iter(path);
  (*i)[cols_adr->text] = new_text;
}

void EditWindow::_on_save_activate()
{
  update_data();
  data.write_back();
}

void EditWindow::_on_close_activate()
{
  hide_all();
}
