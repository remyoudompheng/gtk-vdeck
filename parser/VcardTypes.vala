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
	is_set = true;
      } else {
	types = Cardinal.split(name[1].substring(5), ",");
	is_set = true;
      }
    }
    public string title { get; set; default = ""; }
    public bool is_set { get; set; default = false; }
    public List<string>? types;

    public abstract string str { owned get; set; }
    public string to_string() {
      string t = title;
      if(types != null)
        t += ";TYPE=" + Cardinal.join(",", types);
      return t + ":" + str + "\n";
    }
  }

  public class SimpleField : Field {
    public SimpleField.from_string(string s) { Field.from_string(s); }
    public override string str { owned get; set; }
  }

  public class CompoundField : Field {
    public CompoundField.from_string(string s) { Field.from_string(s); }
    public string[] _content;
    public override string str {
      set { _content = value.split(";"); }
      owned get { return string.joinv(";", _content); }
    }
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
