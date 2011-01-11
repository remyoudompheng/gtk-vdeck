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

using Cardinal;

string library_dir;
bool show_version;

const OptionEntry[] options = {
    { "library", 'd', OptionFlags.FILENAME, OptionArg.FILENAME, out library_dir, "Library path", "lib" },
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

  if (show_version) {
    stdout.puts(Vdeck.PACKAGE_NOTICE);
    return 0;
  }

  if ((library_dir == null) || (args.length == 1)) {
    /* no library or no argument given */
    stderr.puts(oc.get_help(false, null));
    return 0;
  }

  string needle = args[1];
  var deck = new Cardinal.Vdeck.from_directory(library_dir);
  bool found = false;
  stdout.printf("Searching for %s in %s", needle, library_dir);
  foreach(Vcard v in deck.items) {
    if (matches(v, needle)) {
      found = true;
      stdout.puts(mutt_format(v));
    }
  }
  return 0;
}

static bool matches(Vcard v, string needle) {
  return true;
}

static string mutt_format(Vcard v)
{
  return "\n";
}
