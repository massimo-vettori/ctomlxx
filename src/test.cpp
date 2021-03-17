#include <ctomlxx.h>
#include <iostream>

using std::cout;
using std::endl;

using namespace toml;

int main() {
  try {
    Param b("bool   = true");
    Param i("int    = 99");
    Param f("float  = -98.5462");
    Param s("string = \"Hello there!!!\"");

    cout << b.get_name() << ": " << b.to_bool() << endl;
    cout << i.get_name() << ": " << i.to_int() << endl;
    cout << f.get_name() << ": " << f.to_float() << endl;
    cout << s.get_name() << ": " << s.to_string() << endl;
  }

  catch (toml::invalid_type_cast itc) {
    cout << itc.reason << endl;
  }

  catch (toml::name_error ne) {
    cout << ne.reason << endl;
    cout << "Details: {" << ne.name << " - caused the exception throw}" << endl;
  }
}
