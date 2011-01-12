using Cardinal;

public delegate string TestFunc(string in);

static void reciprocaltest(string title, TestFunc func, string in) {
  string out = func(in);
  stdout.printf("%s:\n%s\n%s\n\n", title, in.escape(""), @out.escape(""));
  if (in != out) {
    stderr.puts("input and output values differ !\n");
    Process.exit(1);
  }
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
    stderr.printf("formatted => %s\n", f.to_string());
    return f.str;
  }, "value of the field: hello; how are you?!");

  return 0;
}
