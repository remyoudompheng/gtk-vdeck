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
#include <getopt.h>

#include <iostream>
#include <string>
#include <cassert>

#include "MainWindow.hpp"

/* XML definition file */
#include <vdeck_xml.h>

using namespace std;

string notice = "gtk-vdeck 0.0.1\n"
  "Copyright (C) 2010 Rémy Oudompheng\n"
  "This program comes with ABSOLUTELY NO WARRANTY.\n"
  "This is free software, and you are welcome to redistribute it\n"
  "under certain conditions. See COPYING file for details.\n";

int
main (int argc, char *argv[])
{
  // Options
  static option longopts[] = {
    { "library", 1, 0, 'd' },
    { "version", no_argument, 0, 'V' },
    {0, 0, 0, 0}
  };
  char c;
  string library_dir = "";
  c = getopt_long(argc, argv, "d:V", longopts, &optind);

  switch(c) {
  case 'd':
    library_dir = optarg;
    cout << "Library folder is " << library_dir << endl;
    break;
  case 'V': // Output copyright notice
    cout << notice;
    return 0;
    break;
  }

  Gtk::Main kit(argc, argv);
  Glib::ustring uidef(vdeck_xml, vdeck_xml_len);
  Glib::RefPtr<Gtk::Builder> refGlade = Gtk::Builder::create();
  bool ok = refGlade->add_from_string(uidef);
  assert(ok);

  MainWindow *main_win = 0;
  refGlade->get_widget_derived("main_win", main_win);

  if (main_win) {
    main_win->show_all();
    kit.run(*main_win);
  }

  delete main_win;

  return 0;
}
