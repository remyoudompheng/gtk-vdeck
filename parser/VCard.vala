namespace Cardinal {
  public class VCard : Object {
    /* Constructors and destructors */
    // public VCard()
    // public ~VCard() {}

    /** @brief Full path of the associated vCard file.
     *
     * The string filepath is supposed to refer to an existing
     * file which will be overwritten upon calling write_back.
     * @see write_back()
     */
    string filepath;

    /** Reads a vCard file into memory
     * @param filename The path of the file to be read
     */
    public void open(string filename) {
      filepath = filename;
      FileStream? source = FileStream.open(filename, "r");
      while(!source.eof())
	{
	  string? l = source.read_line();
	  read_field(l);
	}
    }

    public void read_field(string? line) {
      if (line == null) return;
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
      default: break;
      }
    }

    public void write(FileStream fd) {
      fd.puts("BEGIN:VCARD\n");
      /* mandatory fields */
      fd.puts("VERSION:3.0\n");
      fd.puts(uid.to_string());
      fd.puts(fullname.to_string());
      fd.puts(name.to_string());
      // section 3.1
      fd.puts(nickname.to_string());
      fd.puts(photo.to_string());
      fd.puts(birthday.to_string());
      // section 3.2
      foreach(var a in adr.items) fd.puts(a.to_string());
      fd.puts(label.to_string());
      // section 3.3
      foreach(var a in tel.items) fd.puts(a.to_string());
      foreach(var a in email.items) fd.puts(a.to_string());
      fd.puts(mailer.to_string());
      // section 3.4
      fd.puts(tz.to_string());
      fd.puts(geo.to_string());
      // section 3.5
      fd.puts(title.to_string());
      fd.puts(role.to_string());
      fd.puts(logo.to_string());
      fd.puts(agent.to_string());
      fd.puts(org.to_string());
      // section 3.6
      fd.puts(categories.to_string());
      fd.puts(note.to_string());
      fd.puts(prodid.to_string());
      fd.puts(rev.to_string());
      fd.puts(sort_string.to_string());
      fd.puts(sound.to_string());
      fd.puts(uid.to_string());
      fd.puts(url.to_string());
      fd.puts("END:VCARD\n");
    }
    // sec 3.1: identification types
    // FN, N, NICKNAME, PHOTO, BDAY
    public SimpleField fullname { get; set; default = new SimpleField();}
    public CompoundField name { get; set; default = new CompoundField();}
    public SimpleField nickname { get; set; default = new SimpleField();}
    public SimpleField photo { get; set; default = new SimpleField();}
    public SimpleField birthday { get; set; default = new SimpleField();}
    // sec 3.2: delivery addr. types
    // ADR, LABEL
    public CompoundFieldList adr { get; set; default = new CompoundFieldList();}
    public string label { get; set; default = ""; }
    // sec 3.3
    // TEL, EMAIL, MAILER
    public SimpleFieldList tel { get; set; default = new SimpleFieldList();}
    public SimpleFieldList email { get; set; default = new SimpleFieldList();}
    public SimpleField mailer { get; set; default = new SimpleField();}
    // sec 3.4
    // TZ, GEO
    public SimpleField tz { get; set; default = new SimpleField();}
    public CompoundField geo { get; set; default = new CompoundField();}
    // sec 3.5
    // TITLE, ROLE, LOGO, AGENT, ORG
    public SimpleField title { get; set; default = new SimpleField();}
    public SimpleField role { get; set; default = new SimpleField();}
    public SimpleField logo { get; set; default = new SimpleField();}
    public SimpleField agent { get; set; default = new SimpleField();}
    public SimpleField org { get; set; default = new SimpleField();}
    // sec 3.6
    // CATEGORIES, NOTE, PRODID, REV, SORT_STRING, SOUND, UID, URL, VERSION
    public ListField categories { get; set; default = new ListField();}
    public SimpleField note { get; set; default = new SimpleField();}
    public SimpleField prodid { get; set; default = new SimpleField();}
    public SimpleField rev { get; set; default = new SimpleField();}
    public SimpleField sort_string { get; set; default = new SimpleField();}
    public SimpleField sound { get; set; default = new SimpleField();}
    public SimpleField uid { get; set; default = new SimpleField();}
    public SimpleField url { get; set; default = new SimpleField();}
    public SimpleField version { get; set; default = new SimpleField();}
  }

}
