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

#include <gtkmm.h>
#include <iostream>
#include <string>

#include "EditWindow.hpp"
#include <config.h>

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
  gboolean show_version = false;
  static GOptionEntry options[] =
  {
    { "version", 'V', 0, G_OPTION_ARG_NONE, &show_version, "Show version information", NULL },
    { NULL }
  };

  optcontext = g_option_context_new ("- edits a vCard");
  g_option_context_add_main_entries (optcontext, options, NULL);
  g_option_context_add_group (optcontext, gtk_get_option_group (TRUE));
  if (!g_option_context_parse (optcontext, &argc, &argv, &error))
    {
      g_print ("Wrong arguments: %s\n", error->message);
      cout << g_option_context_get_help(optcontext, FALSE, NULL);
      exit (1);
    }

  if (show_version) {
    cout << notice;
    return 0;
  }

  std::string filename;
  if(argc > 0)
    filename = argv[1];
  else {
    cout << g_option_context_get_help(optcontext, FALSE, NULL);
    exit (1);
  }

  // Build up GUI
  Gtk::Main kit(argc, argv);
  EditWindow *main_win = get_with_builder();
  
  if (main_win) {
    main_win->set_path(filename);
    main_win->show_all();
    kit.run(*main_win);
  }

  delete main_win;

  return 0;
}
