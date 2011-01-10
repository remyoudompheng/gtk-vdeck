/*
 * gtk-vdeck
 * A viewer and editor for collections of VCards
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

using Gtk;
using Cardinal;

namespace Vdeck {
  class EditWindow : Window {
    public EditWindow.with_builder() {
      try {
	builder = new Builder ();
	builder.add_from_file (Path.build_filename(RESOURCE_DIR,"editor.xml"));
	builder.connect_signals (null);

	win = builder.get_object ("editor_win") as Window;
      } catch (Error e) {
	stderr.printf ("Could not load UI: %s\n", e.message);
	win = null;
	return;
      }

      /* address TreeView */
      var tree_adr = builder.get_object("tree_adr") as TreeView;
      var store_adr = new ListStore(2, typeof(string), typeof(string));
      tree_adr.set_model(store_adr);

      /* phone TreeView */
      var tree_phone = builder.get_object("tree_phone") as TreeView;
      var store_phone = new ListStore(2, typeof(string), typeof(string));
      tree_phone.set_model(store_phone);

      /* email treeView */
      var tree_email = builder.get_object("tree_email") as TreeView;
      var store_email = new ListStore(2, typeof(string), typeof(string));
      tree_email.set_model(store_email);

      /* editable cells */
      CellRendererText cell;
      cell = builder.get_object("treecell_a_type") as CellRendererText;
      cell.edited.connect( (path, text) => {
	TreeIter iter;
	store_adr.get_iter_from_string(out iter, path);
	store_adr.set_value(iter, 0, text);
      });
      cell = builder.get_object("treecell_a_text") as CellRendererText;
      cell.edited.connect( (path, text) => {
	TreeIter iter;
	store_adr.get_iter_from_string(out iter, path);
	store_adr.set_value(iter, 1, text);
      });

      cell = builder.get_object("treecell_p_type") as CellRendererText;
      cell.edited.connect( (path, text) => {
	TreeIter iter;
	store_phone.get_iter_from_string(out iter, path);
	store_phone.set_value(iter, 0, text);
      });
      cell = builder.get_object("treecell_p_no") as CellRendererText;
      cell.edited.connect( (path, text) => {
	TreeIter iter;
	store_phone.get_iter_from_string(out iter, path);
	store_phone.set_value(iter, 1, text);
      });

      cell = builder.get_object("treecell_e_type") as CellRendererText;
      cell.edited.connect( (path, text) => {
	TreeIter iter;
	store_email.get_iter_from_string(out iter, path);
	store_email.set_value(iter, 0, text);
      });
      cell = builder.get_object("treecell_email") as CellRendererText;
      cell.edited.connect( (path, text) => {
	TreeIter iter;
	store_email.get_iter_from_string(out iter, path);
	store_email.set_value(iter, 1, text);
      });

      /* actions */
      Action act;
      act = builder.get_object("act_close") as Action;
      act.activate.connect( () => { win.destroy(); } );
    }

    public Window? win;

    private Builder builder;
    private Vcard data;
    private string filepath;

    public void open_path(string filename) {
      filepath = filename;
      data = new Vcard.from_file(filename);
      update_display();
    }

    private void update_display() {
      /* page 1 */
      set_text("entry_fullname", data.fullname.str);
      if (data.name.length >= 5) {
	set_text("entry_familyN", data.name.get(0));
	set_text("entry_firstN", data.name.get(1));
	set_text("entry_addN", data.name.get(2));
	set_text("entry_prefix", data.name.get(3));
	set_text("entry_suffix", data.name.get(4));
      } else {
	stderr.printf("Invalid N field : %s of size %d\n", data.name.str, data.name.length);
      }
      set_text("entry_nickname", data.nickname.str);
      set_text("entry_uid", data.uid.str);
      set_text("entry_cats", data.categories.str);
      set_text("entry_bday", data.birthday.str);
      /* page 2 */
      set_text("entry_tz", data.tz.str);
      set_text("entry_geo", data.geo.str);
    }

    private void set_text(string objname, string? text) {
      if (text != null) {
	var obj = builder.get_object(objname) as Entry;
	obj.set_text(text);
      }
    }

    private string? get_text(string objname) {
      var obj = builder.get_object(objname) as Entry;
      if(obj != null) {
	return obj.get_text();
      } else {
	return null;
      }
    }

  }
}
