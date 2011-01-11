/*
 * gtk-vcard
 * A viewer and editor for collections of VCards
 *
 * Copyright (C) 2010-2011 Rémy Oudompheng
 *
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

namespace Cardinal {
  public class Vdeck : Object {
    public Vdeck.from_directory(string path) {
      import(path);
    }

    private string dirpath;
    public List<Vcard> items;

    /** Find .vcf files in the directory specified by path
     * @param path The path to the directory to be searched.
     */
    public void import(string path) {
      dirpath = path;
      walk_in_dirs(path);
    }

    /** Recursively adds file from a directory. This doesn't check for
     * infinite loops due to symlinks.
     * @param path Directory being searched
     */
    private void walk_in_dirs(string path) {
      Dir directory;
      try {
	directory = Dir.open(path);
      }
      catch(FileError e) {
	stderr.puts("FileError: unable to open directory " + path + ".");
	return;
      }

      if (directory == null) return;
      
      unowned string file;
      while ((file = directory.read_name()) != null) {
	string fullpath = Path.build_filename(path, file);
	if (file.has_suffix(".vcf") && !FileUtils.test(fullpath,FileTest.IS_DIR)) {
	  Vcard v = new Vcard.from_file(fullpath);
          /* extract relative path */
          v.relpath = fullpath.substring(dirpath.length + 1);
	  stderr.puts("Found Vcard " + file + "\n");
	  items.append(v);
	}
	if(FileUtils.test(fullpath, FileTest.IS_DIR)) {
	  stderr.puts("Found directory " + file + "\n");
	  walk_in_dirs(fullpath);
	}
      }                      
    }
  }
}
