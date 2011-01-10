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
  public class DeckView {
    public unowned TreeView widget;

    public DeckView.from_builder(Builder b) {
      try {
        widget = b.get_object ("tree_main") as TreeView;
      } catch (Error e) {
        stderr.printf ("Could not load widget: %s\n", e.message);
        widget = null;
        return;
      }
    }
  }
}
