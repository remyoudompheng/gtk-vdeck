#include <VCard.hpp>
#include <VDeck.hpp>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
  if (argc < 2) {
    cout << "Usage: test example.vcf example.dir" << endl;
    return 0;
  }

  VCard test(argv[1]);
  cout << test;

  VDeck testdeck(argv[2]);
  cout << testdeck;

  return 0;
}
