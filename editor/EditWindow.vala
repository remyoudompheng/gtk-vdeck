/*
 * gtk-vdeck
 * A viewer and editor for collections of VCards
 *
 * Copyright (C) 2010-2011 Rémy Oudompheng

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
  public class EditWindow {
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
      store_adr = new ListStore(2, typeof(string), typeof(string));
      tree_adr.set_model(store_adr);

      /* phone TreeView */
      var tree_phone = builder.get_object("tree_phone") as TreeView;
      store_phone = new ListStore(2, typeof(string), typeof(string));
      tree_phone.set_model(store_phone);

      /* email treeView */
      var tree_email = builder.get_object("tree_email") as TreeView;
      store_email = new ListStore(2, typeof(string), typeof(string));
      tree_email.set_model(store_email);

      /* editable cells */
      unowned CellRendererText cell;
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
      unowned Gtk.Action act;
      /* add/remove email */
      act = builder.get_object("act_email_add") as Gtk.Action;
      act.activate.connect( () => { TreeIter i; store_email.append(out i); });
      act = builder.get_object("act_email_remove") as Gtk.Action;
      act.activate.connect( () => { TreeIter i;
        tree_email.get_selection().get_selected(null, out i);
        store_email.remove(i);
      });
      /* add/remove address */
      act = builder.get_object("act_addr_add") as Gtk.Action;
      act.activate.connect( () => { TreeIter i;
        store_adr.append(out i);
        store_adr.set_value(i, 1, ";;;;;;");
      });
      act = builder.get_object("act_addr_remove") as Gtk.Action;
      act.activate.connect( () => { TreeIter i;
        tree_adr.get_selection().get_selected(null, out i);
        store_adr.remove(i);
      });
      /* add/remove phone number */
      act = builder.get_object("act_phone_add") as Gtk.Action;
      act.activate.connect( () => { TreeIter i; store_phone.append(out i); });
      act = builder.get_object("act_phone_remove") as Gtk.Action;
      act.activate.connect( () => { TreeIter i;
        tree_phone.get_selection().get_selected(null, out i);
        store_phone.remove(i);
      });
      /* save, close */
      act = builder.get_object("act_save") as Gtk.Action;
      act.activate.connect( () => {
        update_data();
        FileStream? f = FileStream.open(filepath, "w");
        assert (f != null); data.write(f);
        data.write(stdout); } );
      act = builder.get_object("act_close") as Gtk.Action;
      act.activate.connect( () => { win.destroy(); } );
    }

    /* Widgets */
    private Builder builder;
    public unowned Window? win;
    private ListStore store_adr;
    private ListStore store_phone;
    private ListStore store_email;

    private Vcard data;
    private string filepath;

    public void open_path(string filename) {
      filepath = filename;
      data = new Vcard.from_file(filename);
      update_display();
    }

    /**
     * Fills various widgets with the fields of "data"
     */
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
      store_adr.clear();
      foreach(CompoundField i in data.adr.items) {
        TreeIter iter; store_adr.append(out iter);
        store_adr.set(iter, 0, join(",", i.types), 1, i.str); 
      }
      store_phone.clear();
      foreach(SimpleField i in data.tel.items) {
        TreeIter iter; store_phone.append(out iter);
        store_phone.set(iter, 0, join(",", i.types), 1, i.str); 
      }
      set_text("entry_tz", data.tz.str);
      set_text("entry_geo", data.geo.str);
      /* page 3 */
      store_email.clear();
      foreach(SimpleField i in data.email.items) {
        TreeIter iter; store_email.append(out iter);
        store_email.set(iter, 0, join(",", i.types), 1, i.str); 
      }
      set_text("entry_url", data.url.str);
    }

    /**
     * Fills the Vcard with information entered in widgets
     */
    private void update_data() {
      /* page 1 */
      data.fullname.str = get_text("entry_fullname");
      data.name._content = {
        get_text("entry_familyN"),
        get_text("entry_firstN"),
        get_text("entry_addN"),
        get_text("entry_prefix"),
        get_text("entry_suffix")
      };
      data.nickname.str = get_text("entry_nickname");
      data.uid.str = get_text("entry_uid");
      data.categories.str = get_text("entry_cats");
      data.birthday.str = get_text("entry_bday");
      /* page 2 */
      data.adr.items = new List<CompoundField>();
      store_adr.foreach( (model, path, iter) => {
        string type; string val;
        model.get(iter, 0, out type, 1, out val);
        data.adr.add("ADR;TYPE=" + type + ":" + val);
        return false; // don't stop here
      });

      data.tel.items = new List<SimpleField>();
      store_phone.foreach( (model, path, iter) => {
        string type; string val;
        model.get(iter, 0, out type, 1, out val);
        data.tel.add("TEL;TYPE=" + type + ":" + val);
        return false; // don't stop here
      });

      data.tz.str = get_text("entry_tz");
      data.geo.str = get_text("entry_geo");

      /* page 3 */
      data.email.items = new List<SimpleField>();
      store_email.foreach( (model, path, iter) => {
        string type; string val;
        model.get(iter, 0, out type, 1, out val);
        data.tel.add("EMAIL;TYPE=" + type + ":" + val);
        return false; // don't stop here
      });

      data.url.str = get_text("entry_url");
    }

    /**
     * Sets the text of a Gtk.Entry in builder.
     * @param objname The name of the widget in the GtkBuilder
     * @param text    String of text
     */
    private void set_text(string objname, string? text) {
      if (text != null) {
	var obj = builder.get_object(objname) as Entry;
	obj.set_text(text);
      }
    }

    private unowned string get_text(string objname) {
      var obj = builder.get_object(objname) as Entry;
      if (obj == null) 
        return "";
      else
        return obj.text;
    }

  }
}
