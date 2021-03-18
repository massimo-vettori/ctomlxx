#include <ctomlxx.h>
#include <iostream>

using std::cout;
using std::endl;

using namespace toml;

int main() {
  try {
    Root r;

    Parser p;
    p.load("./file.toml");
    p.parse(r);
    p.flush();

    for (size_t i=0; i<r.at(0).size(); i++) {
      const Param& p = r.at(0).at(i);
      switch(p.type_id()) {
        case BOOL:   cout << p.get_name() << " >>> " << p.to_bool() << endl; break;
        case INT:    cout << p.get_name() << " >>> " << p.to_int() << endl; break;
        case FLOAT:  cout << p.get_name() << " >>> " << p.to_float() << endl; break;
        case STRING: cout << p.get_name() << " >>> " << p.to_string() << endl; break;

        default: break;
      }
    }

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
