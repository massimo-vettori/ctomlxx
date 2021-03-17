#include <ctomlxx.h>
#include <iostream>

using std::cout;
using std::endl;

using namespace toml;

int main() {
  try {
    Section s(" [SectionTest001]  ");
    s.append("      bool_value      =     true    ");
    s.append("  float_value   =     10.21   ");
    
    Root r;
    r.append(s);

    cout << r.at(0).get("float_value").to_float() << endl;
    cout << s.at(0).to_bool() << endl;
  }

  catch (toml::invalid_type_cast itc) {
    cout << itc.reason << endl;
  }

  catch (toml::name_error ne) {
    cout << ne.reason << endl;
    cout << "Details: {'" << ne.name << "' - caused the exception throw}" << endl;
  }

  catch (...) {
    cout << "Uncaught error occurred..." << endl;
  }
}
