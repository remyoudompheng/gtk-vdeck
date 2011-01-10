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
    }

    public void set_path(string libpath) {
    }

  }
}
