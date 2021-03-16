#include <ctomlxx.h>
#include <iostream>

using std::cout;
using std::endl;

using namespace toml;

int main() {
  try {
  }

  catch (toml::invalid_type_cast itc) {
    cout << itc.reason << endl;
  }
}
