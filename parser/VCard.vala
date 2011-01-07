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
      fd.puts(fullname.to_string());
      fd.puts(name.to_string());
      foreach(var a in adr.items)
	fd.puts(a.to_string());
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
  }

}
