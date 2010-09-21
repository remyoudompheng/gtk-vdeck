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

#include "VDeck.hpp"
#include <config.h>
#include <iostream>

using namespace std;

string notice = PACKAGE_STRING "\n"
  PACKAGE_COPYRIGHT "\n"
  "This program comes with ABSOLUTELY NO WARRANTY.\n"
  "This is free software, and you are welcome to redistribute it\n"
  "under certain conditions. See COPYING file for details.\n";

int
main (int argc, char *argv[])
{
  // Options
  GError *error = NULL;
  GOptionContext *optcontext;
  gchar *library_dir = NULL;
  gboolean show_version = false;
  static GOptionEntry options[] =
  {
    { "library", 'd', G_OPTION_FLAG_FILENAME, G_OPTION_ARG_FILENAME,
      &library_dir, "Library path", "lib" },
    { "version", 'V', 0, G_OPTION_ARG_NONE,
      &show_version, "Show version information", NULL },
    { NULL }
  };

  optcontext = g_option_context_new ("- searches through vCards");
  g_option_context_add_main_entries (optcontext, options, NULL);

  // check for errors
  if (!g_option_context_parse (optcontext, &argc, &argv, &error))
    {
      g_print ("Wrong arguments: %s\n", error->message);
      cout << g_option_context_get_help(optcontext, FALSE, NULL);
      exit (1);
    }

  // show version
  if (show_version) {
    cout << notice;
    return 0;
  }

  // no library folder specified
  if(!library_dir) {
    cout << g_option_context_get_help(optcontext, FALSE, NULL);
    return 0;
  }

  std::string search_str;
  if(argc > 1)
    search_str = argv[1];
  else {
    // no search string specified
    cout << g_option_context_get_help(optcontext, FALSE, NULL);
    exit (1);
  }

  // let's start
  VDeck v;
  v.import_dir(library_dir);
  cout << "Searching for '" << search_str << "' in " << library_dir << endl;
  bool found = false;
  for(VDeck::iterator c = v.begin();
      c != v.end(); c++) {
    if(c->matches(search_str)) {
      cout << c->mutt_format();
      found = true;
    }
  }

  return (found ? 0 : 1);
}
