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

using namespace std;

/** Find .vcf files in the directory specified by path
 * @param path The path to the directory to be searched.
 */
void VDeck::import_dir(const string path)
{

  // Recursively adds file from a directory. This doesn't check for
  // infinite loops due to symlinks.
  try {
    Glib::Dir directory(path);
    for(Glib::DirIterator it = directory.begin();
        it != directory.end(); it++)
      {
        if (Glib::file_test(path + G_DIR_SEPARATOR + *it, Glib::FILE_TEST_IS_DIR))
          {
            import_dir(path + G_DIR_SEPARATOR + *it);
          }
        else
	  if ((*it).compare((*it).size() - 4, 4, ".vcf") == 0)
	    {
#ifdef DEBUG
	      cout << path << G_DIR_SEPARATOR << *it << endl;
#endif
	      insert(VCard((path + G_DIR_SEPARATOR + *it).c_str()));
	    }
      }
  }
  catch(const Glib::FileError& ex) {
    std::cerr << "FileError: " << ex.what() << std::endl;
    return;
  }
}

void VDeck::create_new(const std::string path)
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
  return out;
}
