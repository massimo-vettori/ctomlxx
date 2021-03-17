#include <ctomlxx.h>
#include <iostream>

using std::cout;
using std::endl;

using namespace toml;

int main() {
  try {
    Root r;

    Parser p;
    p.load("./local/test.toml");
    p.parse(r);
    p.flush();

    cout << r.get("test").get("nth").to_int() << endl;
    cout << r.get("test").get("str").to_string() << endl;
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
