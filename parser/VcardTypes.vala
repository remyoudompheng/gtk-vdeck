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
  // Helper method
  public static List<string> split(string s, string d) {
    var l = new List<string> ();
    foreach(string i in s.split(d))
      l.append(i);
    return l;
  }

  public static string join(string d, List<string> s) {
    string[] l = {};
    foreach(string i in s)
      l += i;
    return string.joinv(d, l);
  }

  public abstract class Field {
    public Field.from_string(string s) {
      // Read content
      string[] chunks = s.split(":", 2);
      assert(chunks.length == 2);
      str = chunks[1];
      /* read title */
      string[] name = chunks[0].split(";");
      title = name[0];
      if (name.length == 1) {
	types = null;
      } else {
	types = Cardinal.split(name[1].substring(5), ",");
      }
    }
    public string title { get; set; default = ""; }
    public abstract bool is_set { get; }
    public List<string>? types;

    public abstract string str { owned get; set; }

    /**
     * Convert the field to a formatted string
     * inverse of from_string(string)
     */
    public string to_string() {
      if(! is_set) return "";
      string t = title;
      if(types != null)
	t += ";TYPE=" + Cardinal.join(",", (!)types);
      return t + ":" + str;
    }

    public string to_line() {
      if (!is_set) return "";
      else return this.to_string() + "\n";
    }
  }

  public class SimpleField : Field {
    public SimpleField(string t) { title = t; }
    public SimpleField.from_string(string s) { Field.from_string(s); }
    public override string str { owned get; set; default = "";}
    public override bool is_set { get { return (str.length > 0); } }
  }

  public class CompoundField : Field {
    public CompoundField(string t, int size) { title = t; _content = new string[size]; }
    public CompoundField.from_string(string s) { Field.from_string(s); }
    public string[]? _content;
    public override string str {
      /* TODO : treat escaped semicolons */
      set { _content = value.split(";"); }
      owned get { return string.joinv(";", _content); }
    }
    public string get(int i) { return _content[i]; }
    public void set(int i, string value) { _content[i] = value; }
    public int length {
      get { return _content.length; }
      set { _content.resize(value); }
    }
    public override bool is_set { get {
      if (_content == null) return false;
      foreach(string i in _content) {
        if ((i != null) && (i.length > 0)) return true;
      }
      return false;
    }}
  }

  public class ListField : Field {
    public ListField(string t) { title = t; }
    public ListField.from_string(string s) { Field.from_string(s); }
    public string[] _content;
    public override string str {
      /* TODO : treat escaped colons */
      set { _content = value.split(","); }
      owned get { return string.joinv(",", _content); }
    }
    public string field(int i) { return _content[i]; }
    public override bool is_set { get { return (_content != null) && (_content.length > 0); } }
  }

  public class SimpleFieldList {
    public SimpleFieldList() {
      items = new List<SimpleField>() ;
    }
    public string title { get; set; default = ""; }
    public List<SimpleField> items;

    public void add(string s) {
      var f = new SimpleField.from_string(s);
      title = f.title;
      items.append(f);
    }
  }

  public class CompoundFieldList {
    public CompoundFieldList() {
      items = new List<CompoundField>() ;
    }
    public string title { get; set; default = ""; }
    public List<CompoundField> items;

    public void add(string s) {
      var f = new CompoundField.from_string(s);
      title = f.title;
      items.append(f);
    }
  }
}
