using Cardinal;

public delegate string TestFunc(string in);

static void reciprocaltest(string title, TestFunc func, string in) {
  string out = func(in);
  if (in != out) stderr.puts("input and output values differ !\n");
  stdout.printf("%s:\n%s\n%s\n\n", title, in.escape(""), @out.escape(""));
  // if (in != out) Process.exit(1);
}

static int main(string[] args)
{
  /**
   * VcardTypes
   */
  stdout.printf("VcardTypes\n----------\n");

  /* split/join function */
  reciprocaltest("split/join", (s) => {
    var l = Cardinal.split(s, ",");
    return Cardinal.join(",", l);
  }, "a,b,c,abc");

  /* SimpleField */
  reciprocaltest("simple field", (s) => {
    var f = new SimpleField.from_string(s);
    stderr.printf("value ==> %s\n", f.str);
    stderr.printf("types ==> %s\n", Cardinal.join(",", f.types));
    return f.to_string();
  }, "FIELD;TYPE=T1,T2:value of the field: hello; how are you?!");

  reciprocaltest("simple field value", (s) => {
    var f = new SimpleField("TITLE");
    f.str = s;
    stderr.printf("formatted => %s\n", f.to_string().escape(""));
    return f.str;
  }, "value of the field: hello; how are you?!");

  /* CompoundField */
  reciprocaltest("compound field", (s) => {
    var f = new CompoundField.from_string(s);
    stderr.printf("values ==> %s\n", string.joinv(", ", f._content));
    return f.to_string();
  }, "FIELD:value1;value2;value3");

  reciprocaltest("compound field with escaped semicolons", (s) => {
    var f = new CompoundField.from_string(s);
    stderr.printf("values ==> %s\n", string.joinv(", ", f._content));
    return f.to_string();
  }, "FIELD:this\\;is\\;value1;value2;value3");

  string vcard_example =
"""BEGIN:VCARD
VERSION:3.0
N:Gump;Forrest
FN:Forrest Gump
ORG:Bubba Gump Shrimp Co.
TITLE:Shrimp Man
PHOTO;VALUE=URL;TYPE=GIF:http://www.site.com/dir_photos/my_photo.gif
TEL;TYPE=WORK,VOICE:(111) 555-1212
TEL;TYPE=HOME,VOICE:(404) 555-1212
ADR;TYPE=WORK:;;100 Waters Edge;Baytown;LA;30314;United States of America
LABEL;TYPE=WORK:100 Waters Edge\nBaytown, LA 30314\nUnited States of America
ADR;TYPE=HOME:;;42 Plantation St.;Baytown;LA;30314;United States of America
LABEL;TYPE=HOME:42 Plantation St.\nBaytown, LA 30314\nUnited States of America
EMAIL;TYPE=PREF,INTERNET:forrestgump@example.com
REV:20080424T195243Z
END:VCARD
""" ;

  /* Vcard */
  stdout.puts("vcard parsing\n");
  stdout.puts("input :\n" + vcard_example);
  var v = new Vcard.from_string(vcard_example);
  stdout.puts("output:\n" + v.to_string());

  return 0;
}
