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
#include <StructuredText.hpp>
#include <iostream>
#include <cassert>

#include <config.h>

using namespace std;

typedef Glib::RefPtr<Gtk::CellRendererText> CellPtr;

EditWindow* get_with_builder()
{
  Glib::RefPtr<Gtk::Builder> refGlade;
  try {
    refGlade = Gtk::Builder::create_from_file(RESOURCE_DIR "/editor.xml");
  }
  catch (Glib::FileError& ex)
    {
      cerr << "GtkBuilder creation failed: " << ex.what() << endl;
      return NULL;
    }

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
  CellPtr cell_a_type = CellPtr::cast_static(uidef->get_object("treecell_a_type"));
  cell_a_type->signal_edited().connect(sigc::mem_fun(*this, &EditWindow::_on_edited_adr_type));
  CellPtr cell_addr = CellPtr::cast_static(uidef->get_object("treecell_a_text"));
  cell_addr->signal_edited().connect(sigc::mem_fun(*this, &EditWindow::_on_edited_adr));

  // Telephone TreeView
  uidef->get_widget("tree_phone", tree_phone);
  cols_phone = new PhoneColumns;
  store_phone = Gtk::ListStore::create(*cols_phone);
  tree_phone->set_model(store_phone);

  // Editable cells
  CellPtr cell_p_type = CellPtr::cast_static(uidef->get_object("treecell_p_type"));
  cell_p_type->signal_edited().connect(sigc::mem_fun(*this, &EditWindow::_on_edited_p_type));
  CellPtr cell_p_no = CellPtr::cast_static(uidef->get_object("treecell_p_no"));
  cell_p_no->signal_edited().connect(sigc::mem_fun(*this, &EditWindow::_on_edited_p_no));

  // Email TreeView
  uidef->get_widget("tree_email", tree_email);
  cols_email = new EmailColumns;
  store_email = Gtk::ListStore::create(*cols_email);
  tree_email->set_model(store_email);

  // Editable cells
  CellPtr cell_e_type = CellPtr::cast_static(uidef->get_object("treecell_e_type"));
  cell_e_type->signal_edited().connect(sigc::mem_fun(*this, &EditWindow::_on_edited_e_type));
  CellPtr cell_email = CellPtr::cast_static(uidef->get_object("treecell_email"));
  cell_email->signal_edited().connect(sigc::mem_fun(*this, &EditWindow::_on_edited_email_addr));

  // Actions
  connect_action("act_email_add", &EditWindow::_on_email_add_activate);
  connect_action("act_addr_add", &EditWindow::_on_adr_add_activate);
  connect_action("act_phone_add", &EditWindow::_on_phone_add_activate);
  connect_action("act_email_remove", &EditWindow::_on_email_remove_activate);
  connect_action("act_addr_remove", &EditWindow::_on_adr_remove_activate);
  connect_action("act_phone_remove", &EditWindow::_on_phone_remove_activate);
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
      (*iter)[cols_adr->type] = i->first.join();
      (*iter)[cols_adr->text] = i->second.join();
  }
  store_phone->clear();
  for(VCard::tel_t::iterator i = data.tel.begin();
      i != data.tel.end(); i++) {
      Gtk::TreeIter iter = store_phone->append();
      (*iter)[cols_phone->type] = i->first.join();
      (*iter)[cols_phone->number] = i->second;
  }
  // Page 3
  store_email->clear();
  for(VCard::email_t::iterator i = data.email.begin();
      i != data.email.end(); i++) {
      Gtk::TreeIter iter = store_email->append();
      (*iter)[cols_email->type] = i->first.join();
      (*iter)[cols_email->adr] = i->second;
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
  data.adr.clear();
  for(Gtk::TreeIter i = store_adr->children().begin();
      i != store_adr->children().end(); i++) {
    data.adr.append( CommaStruct((*i)[cols_adr->type]),
		     SemicolonStruct((*i)[cols_adr->text]));
  }
  data.tel.clear();
  for(Gtk::TreeIter i = store_phone->children().begin();
      i != store_phone->children().end(); i++) {
    data.tel.append( CommaStruct((*i)[cols_phone->type]),
		     (*i)[cols_phone->number]);
  }
  // Page 3
  data.email.clear();
  for(Gtk::TreeIter i = store_email->children().begin();
      i != store_email->children().end(); i++) {
    data.email.append( CommaStruct((*i)[cols_email->type]),
		       (*i)[cols_email->adr]);
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
#ifdef DEBUG
  cerr << "Read data:" << endl << data;
#endif
  update_display();
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

/*
 *  Callbacks for add/remove buttons
 */

void EditWindow::_on_email_add_activate() { store_email->append(); }
void EditWindow::_on_phone_add_activate() { store_phone->append(); }
void EditWindow::_on_adr_add_activate()
{
  Gtk::TreeIter i = store_adr->append();
  (*i)[cols_adr->text] = ";;;;;;";
}

void EditWindow::_on_email_remove_activate()
{
  Gtk::TreeView *t;
  uidef->get_widget("tree_email", t);
  store_email->erase(t->get_selection()->get_selected());
}

void EditWindow::_on_phone_remove_activate()
{
  Gtk::TreeView *t;
  uidef->get_widget("tree_phone", t);
  store_phone->erase(t->get_selection()->get_selected());
}

void EditWindow::_on_adr_remove_activate()
{
  Gtk::TreeView *t;
  uidef->get_widget("tree_adr", t);
  store_adr->erase(t->get_selection()->get_selected());
}

/*
 * Call backs for editable cells
 */

void EditWindow::_on_edited_e_type(const Glib::ustring& path, const Glib::ustring& new_text)
{
  Gtk::TreeIter i = store_email->get_iter(path);
  (*i)[cols_email->type] = new_text;
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

void EditWindow::_on_edited_adr_type(const Glib::ustring& path, const Glib::ustring& new_text)
{
  Gtk::TreeIter i = store_adr->get_iter(path);
  (*i)[cols_adr->type] = new_text;
}

void EditWindow::_on_edited_p_type(const Glib::ustring& path, const Glib::ustring& new_text)
{
  Gtk::TreeIter i = store_phone->get_iter(path);
  (*i)[cols_phone->type] = new_text;
}

void EditWindow::_on_edited_p_no(const Glib::ustring& path, const Glib::ustring& new_text)
{
  Gtk::TreeIter i = store_phone->get_iter(path);
  (*i)[cols_phone->number] = new_text;
}
