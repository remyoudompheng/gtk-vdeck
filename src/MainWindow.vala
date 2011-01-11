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
  public class MainWindow {
    public unowned Window win;
    private Builder builder;

    /* widgets */
    private DeckView view;
    private ListStore cat_store;
    private unowned TreeSelection cat_selection;

    public MainWindow.with_builder() {
      try {
        builder = new Builder ();
        builder.add_from_file (Path.build_filename(RESOURCE_DIR,"vdeck.xml"));
        builder.connect_signals (null);

        win = builder.get_object ("main_win") as Window;
      } catch (Error e) {
        stderr.printf ("Could not load UI: %s\n", e.message);
        win = null;
        return;
      }

      // Get the TreeView object
      view = new DeckView.from_builder(builder);

      // categories TreeView
      cat_store = new ListStore(1, typeof(string));
      var cat_view = builder.get_object("tree_cats") as TreeView;
      cat_view.set_model(cat_store);
      var cat_treecol = builder.get_object("treecol_cats") as TreeViewColumn;
      cat_treecol.set_sort_column_id(0);

      // filtering by category/directory
      cat_selection = cat_view.get_selection();
      cat_selection.set_mode(SelectionMode.MULTIPLE);
      cat_selection.changed.connect(this.on_cat_selection_changed);

      // action signals
      unowned Action act;
      act = builder.get_object("act_openlib") as Action;
      act.activate.connect( this.on_openlib_activate );
      act = builder.get_object("act_refresh") as Action;
      act.activate.connect(this.update_library);

      /* TODO: add/remove buttons */

      act = builder.get_object("act_quit") as Action;
      act.activate.connect(Gtk.main_quit);
    }

    private string dir_path;
    private Cardinal.Vdeck deck;

    public void set_path(string libpath) {
      dir_path = libpath;
      update_library();
    }

    /* Loads the Vdeck from the given path */
    private void update_library() {
      deck = new Cardinal.Vdeck.from_directory(dir_path);
      view.fill_data(deck);
      update_cats();
    }

    /* updates the list of categories */
    private void update_cats() {
      var categories = new Gee.HashSet<string>();
      /* add the "[none]" category */
      categories.add(DeckView.NONE);
      foreach(Vcard v in deck.items) {
        foreach(string cat in v.categories._content)
          categories.add(cat);
      }
      // Fill the ListStore
      cat_store.clear();
      foreach(string cat in categories) {
        TreeIter i;
        cat_store.append(out i);
        cat_store.set(i, 0, cat);
      }

      cat_selection.select_all();
    }

    private void on_openlib_activate() {
      var dialog = new FileChooserDialog(
        "Choose the path of the library",
        null, FileChooserAction.SELECT_FOLDER);
      dialog.add_button(STOCK_CANCEL, ResponseType.CANCEL);
      dialog.add_button(STOCK_OPEN, ResponseType.OK);
      dialog.set_default_response(ResponseType.OK);
      if (dialog.run() == ResponseType.OK)
        set_path(dialog.get_filename());
      dialog.destroy();
    }

    /* Category selection changed : update the category filter */
    private void on_cat_selection_changed() {
      view.cat_filter.clear();
      TreeModel m;
      foreach(TreePath i in cat_selection.get_selected_rows(out m)) {
        TreeIter iter;
        if(m.get_iter(out iter, i)) {
          string cat;
          m.get(iter, 0, out cat);
          view.cat_filter.add(cat);
        }
      }
      /* update display */
      view.refilter();
    }

  }
}
