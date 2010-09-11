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

/* XML definition file */
#include "EditWindow.hpp"

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
    { "version", no_argument, 0, 'V' },
    {0, 0, 0, 0}
  };

  char c;
  c = getopt_long(argc, argv, "d:V", longopts, &optind);

  switch(c) {
  case 'V': // Output copyright notice
    cout << notice;
    return 0;
    break;
  }

  std::string filename;
  if(optind < argc)
    filename = argv[optind];
  else {
    cout << "Usage: " << argv[0] << "[-V] filename" << endl;
    cout << "  -V: print version information" << endl;
    cout << "  filename: name of the vCard file to edit" << endl;
    return 0;
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
