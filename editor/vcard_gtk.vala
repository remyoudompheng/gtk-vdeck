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
using Vdeck;

bool show_version;
const OptionEntry[] options = {
    { "version", 'V', 0, OptionArg.NONE, out show_version, "Show version information", null },
    { null }
};

static int main (string[] args) {
  OptionContext oc = new OptionContext(" - edits a vCard");
  oc.set_help_enabled(true);
  oc.add_main_entries(options, null);

  try {
    bool b = oc.parse(ref args);
    if (!b)
      {
        /* option parsing failed */
        stderr.puts(oc.get_help(false, null));
        return 1;
      }

  }
  catch (OptionError e)
  {
    stderr.puts("Unable to parse options : " + e.message + "\n");
    return 1;
  }

  if (args.length == 1) {
    /* no argument given */
    stderr.puts(oc.get_help(false, null));
    return 0;
  }

  string filename = args[1];

  Gtk.init (ref args);
  var editor = new EditWindow.with_builder();
  editor.open_path(filename);
  editor.win.destroy.connect(Gtk.main_quit);
  editor.win.show_all ();
  Gtk.main ();
  return 0;
}
