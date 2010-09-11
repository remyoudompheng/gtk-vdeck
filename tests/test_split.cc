#include <StructuredText.hpp>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
  string s="one\\,two,three,four\\,five";
  cout << "Splitting " << s << endl;
  CommaStruct l(s);
  for(CommaStruct::iterator i = l.begin(); i != l.end(); i++)
    cout << *i << endl;
  cout << "Rejoin: " << l << endl << endl;

  s = "one\\;two;three;four\\;five";
  cout << "Splitting " << s << endl;
  SemicolonStruct m(s);
  for(SemicolonStruct::iterator i = m.begin(); i != m.end(); i++)
    cout << *i << endl;
  cout << "Rejoin: " << m << endl << endl;

  return 0;
}
