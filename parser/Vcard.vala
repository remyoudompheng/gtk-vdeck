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
  public class Vcard : Object {
    /* Constructors and destructors */
    public Vcard() {}
    public Vcard.from_file(string path) { open(path); }
    public Vcard.from_string(string s) { this.read_string(s); }

    // public ~Vcard() {}

    /** @brief Full path of the associated vCard file.
     *
     * The string filepath is supposed to refer to an existing
     * file which will be overwritten upon calling write_back.
     * @see write_back()
     */
    public string filepath { get; set; default = ""; }

    /* relative path of the file */
    public string relpath { get; set; default = ""; }

    /** Reads a vCard file into memory
     * @param filename The path of the file to be read
     */
    public void open(string filename) {
      filepath = filename;
      try {
        string contents;
        bool ok = FileUtils.get_contents(filename, out contents);
        if (!ok) return;
        this.read_string(contents);
      }
      catch (FileError e) {
	stderr.puts("FileError: unable to open file " + filename + ": " + e.message);
	return;
      }
    }

    public void read_string(string source) {
      foreach(string line in source.split("\n")) {
        read_field(line);
      }
    }

    public void read_field(string line) {
      // Split %field:%content
      string[] chunks = line.split(":", 2);
      if(chunks.length < 2) return;
      string[] field = chunks[0].split(";");
      // Fill corresponding field
      switch(field[0]) {
      case "BEGIN":
      case "END":
	if(chunks[1].ascii_casecmp("VCARD") != 0)
	  stderr.printf("Found %s in field %s instead of VCARD", chunks[1], field[0]);
      break;
      case "FN":       fullname = new SimpleField.from_string(line); break;
      case "N":            name = new CompoundField.from_string(line); break;
      case "NICKNAME": nickname = new SimpleField.from_string(line); break;
      case "PHOTO":       photo = new SimpleField.from_string(line); break;
      case "BDAY":     birthday = new SimpleField.from_string(line); break;
      // 3.2
      case "ADR": adr.add(line); break;
      case "LABEL":       label = new SimpleField.from_string(line); break;
      // 3.3
      case "TEL": tel.add(line); break;
      case "EMAIL": email.add(line); break;
      case "MAILER":     mailer = new SimpleField.from_string(line); break;
      // 3.4
      case "TZ":  tz = new SimpleField.from_string(line); break;
      case "GEO": geo = new CompoundField.from_string(line); break;
      // 3.5
      case "TITLE": title = new SimpleField.from_string(line); break;
      case "ROLE":  role = new SimpleField.from_string(line); break;
      case "LOGO":  logo = new SimpleField.from_string(line); break;
      case "AGENT": agent = new SimpleField.from_string(line); break;
      case "ORG":   org = new SimpleField.from_string(line); break;
      // 3.6
      case "CATEGORIES": categories = new ListField.from_string(line); break;
      case "NOTE":   note = new SimpleField.from_string(line); break;
      case "PRODID": prodid = new SimpleField.from_string(line); break;
      case "REV":    rev = new SimpleField.from_string(line); break;
      case "SORT-STRING": sort_string = new SimpleField.from_string(line); break;
      case "SOUND":  sound = new SimpleField.from_string(line); break;
      case "UID":    uid = new SimpleField.from_string(line); break;
      case "URL":    url = new SimpleField.from_string(line); break;
      case "VERSION": version = new SimpleField.from_string(line); break;
      /* TODO: add support for custom fields */
      default: break;
      }
    }

    public string to_string() {
      var output = new StringBuilder();
      output.append("BEGIN:VCARD\n");
      /* mandatory fields */
      output.append("VERSION:3.0\n");
      output.append(uid.to_line());
      output.append(fullname.to_line());
      output.append(name.to_line());
      // section 3.1
      output.append(nickname.to_line());
      output.append(photo.to_line());
      output.append(birthday.to_line());
      // section 3.2
      foreach(var a in adr.items) output.append(a.to_line());
      output.append(label.to_line());
      // section 3.3
      foreach(var a in tel.items) output.append(a.to_line());
      foreach(var a in email.items) output.append(a.to_line());
      output.append(mailer.to_line());
      // section 3.4
      output.append(tz.to_line());
      output.append(geo.to_line());
      // section 3.5
      output.append(title.to_line());
      output.append(role.to_line());
      output.append(logo.to_line());
      output.append(agent.to_line());
      output.append(org.to_line());
      // section 3.6
      output.append(categories.to_line());
      output.append(note.to_line());
      output.append(prodid.to_line());
      output.append(rev.to_line());
      output.append(sort_string.to_line());
      output.append(sound.to_line());
      // output.append(uid.to_line());
      output.append(url.to_line());
      output.append("END:VCARD\n");

      return output.str;
    }

    /**
     * List of fields
     */

    // sec 3.1: identification types
    // FN, N, NICKNAME, PHOTO, BDAY
    public SimpleField fullname { get; set; default = new SimpleField("FN");}
    public CompoundField name { get; set; default = new CompoundField.from_string("N:;;;;");}
    public SimpleField nickname { get; set; default = new SimpleField("NICKNAME");}
    public SimpleField photo { get; set; default = new SimpleField("PHOTO");}
    public SimpleField birthday { get; set; default = new SimpleField("BDAY");}
    // sec 3.2: delivery addr. types
    // ADR, LABEL
    public CompoundFieldList adr { get; set; default = new CompoundFieldList();}
    public SimpleField label { get; set; default = new SimpleField("LABEL"); }
    // sec 3.3
    // TEL, EMAIL, MAILER
    public SimpleFieldList tel { get; set; default = new SimpleFieldList();}
    public SimpleFieldList email { get; set; default = new SimpleFieldList();}
    public SimpleField mailer { get; set; default = new SimpleField("MAILER");}
    // sec 3.4
    // TZ, GEO
    public SimpleField tz { get; set; default = new SimpleField("TZ");}
    public CompoundField geo { get; set; default = new CompoundField("GEO", 2);}
    // sec 3.5
    // TITLE, ROLE, LOGO, AGENT, ORG
    public SimpleField title { get; set; default = new SimpleField("TITLE");}
    public SimpleField role { get; set; default = new SimpleField("ROLE");}
    public SimpleField logo { get; set; default = new SimpleField("LOGO");}
    public SimpleField agent { get; set; default = new SimpleField("AGENT");}
    public SimpleField org { get; set; default = new SimpleField("ORG");}
    // sec 3.6
    // CATEGORIES, NOTE, PRODID, REV, SORT-STRING, SOUND, UID, URL, VERSION
    public ListField categories { get; set; default = new ListField("CATEGORIES");}
    public SimpleField note { get; set; default = new SimpleField("NOTE");}
    public SimpleField prodid { get; set; default = new SimpleField("PRODID");}
    public SimpleField rev { get; set; default = new SimpleField("REV");}
    public SimpleField sort_string { get; set; default = new SimpleField("SORT-STRING");}
    public SimpleField sound { get; set; default = new SimpleField("SOUND");}
    public SimpleField uid { get; set; default = new SimpleField("UID");}
    public SimpleField url { get; set; default = new SimpleField("URL");}
    public SimpleField version { get; set; default = new SimpleField("VERSION");}
  }

}
