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
  /** A Gtk:TreeView derived class to display a VDeck library of VCard structures
   */
  public class DeckView {
    public unowned TreeView widget;
    private ListStore list_store;
    private TreeModelFilter list_filtered;
    private TreeModelSort list_sorted;
    private Builder builder;

    private enum Columns {
      FULLNAME,
      FAMILY,
      FIRST,
      PHONE,
      EMAIL,
      PATH,
      VCARD
    }

    public const string NONE = "[none]";

    public DeckView.from_builder(Builder b) {
      builder = b;
      cat_filter = new Gee.HashSet<string>();
      dir_filter = new Gee.HashSet<string>();

      widget = b.get_object ("tree_main") as TreeView;

      list_store = new ListStore(7, typeof(string), typeof(string),
        typeof(string), typeof(string), typeof(string), typeof(string), typeof(Cardinal.Vcard));

      list_filtered = new TreeModelFilter(list_store, null);
      list_filtered.set_visible_func(this.is_shown_by_filter);

      list_sorted = new TreeModelSort.with_model(list_filtered);
      list_sorted.sort_column_id = Columns.FAMILY;
      widget.set_model(list_sorted);

      // activating rows
      widget.row_activated.connect(this.on_row_activated);
    }

    private EditWindow editor;

    private void on_row_activated (TreePath path, TreeViewColumn column) {
        TreeIter i; Vcard v;
        list_sorted.get_iter(out i, path);
        list_sorted.get(i, Columns.VCARD, out v);

        editor = new EditWindow.with_builder();
        editor.win.set_transient_for(builder.get_object("main_win") as Window);
        editor.open_path(v.filepath);
        editor.win.show_all();
    }

    /* Filters (by category, by folder) */
    public Gee.HashSet<string> cat_filter;
    public Gee.HashSet<string> dir_filter;

    public void refilter() {
      list_filtered.refilter();
    }

    private bool is_shown_by_filter(TreeModel model, TreeIter iter) {
      Vcard v;
      model.get(iter, Columns.VCARD, out v);
      if(v == null) return false;

      /* check folder */
      string dir = Path.get_dirname(v.relpath);
      if (!(dir in dir_filter)) return false;
      /* check category */
      if(v.categories.is_set) {
        foreach(string cat in v.categories._content) {
          if(cat in cat_filter) return true;
        }
        return false;
      } else {
        return (NONE in cat_filter);
      }
    }

    /** Fills the Gtk.ListStore with records
     * @param deck A VDeck structure containing VCards
     */
    public void fill_data(Cardinal.Vdeck deck) {
      list_store.clear();
      foreach(Vcard v in deck.items) {
        TreeIter iter;
        string phone;
        string email;
        if(v.tel.items != null) phone = v.tel.items.data.str;
          else phone = "";
        if(v.email.items != null) email = v.email.items.data.str;
          else email = "";
        list_store.append(out iter);
        list_store.set(iter,
          0, v.fullname.str,
          1, v.name.get(0),
          2, v.name.get(1),
          3, phone,
          4, email,
          5, v.relpath,
          6, v);
      }
    }
  }
}
