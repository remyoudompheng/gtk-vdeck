/*
 * gtk-vcard
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

#include <iostream>
#include <ftw.h>

using namespace std;

VDeck *that;

// Helper function
extern "C" int read_path_callback(const char *path,
			  const struct stat *sb,
			  int typeflag)
{
  string spath = path;
  if (spath.compare(spath.size() - 4, 4, ".vcf") == 0) {
#ifdef DEBUG
    cout << spath << endl;
#endif
    that->insert(VCard(path));
  }
  return 0;
}

#define FD_MAX_USE 5
/** Find .vcf files in the directory specified by path
 * @param path The path to the directory to be searched.
 */
void VDeck::import_dir(string path)
{
  that = this;
  ftw(path.c_str(), read_path_callback, FD_MAX_USE);
}

void VDeck::create_new(std::string path)
{
  VCard v;
  v.filepath = path;
  v.write_back();
  insert(v);
}

/** Print a sequence of vCards from the given structure.
 * @param that The VDeck to print.
 */
ostream& operator<< (ostream &out, VDeck const & that)
{
  VDeck::iterator iter;
  for (iter = that.begin();
       iter != that.end();
       iter++)
    {
      out << *iter;
      out << endl;
    }
}
