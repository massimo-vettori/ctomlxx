#include <ctomlxx.h>
#include <iostream>

using std::cout;
using std::endl;

/* Since this example is using everything possible from the toml namespace */
using namespace toml;

int main() {
  try {
    Root r; /* Creates a new "Root" variable */

    Parser p;              /* Creates a new "Parser" */
    p.load("./file.toml"); /* Loads a local .toml file */
    p.parse(r);            /* Parses the content of the loaded file */
    p.flush();             /* Empties the buffer of the parser (not mandatory) */

    /* Cycle through every section in 'r' */
    for (size_t i=0; i<r.size(); i++) {
      const Section& s = r[i];
      /* Prints section name */
      cout << "[" << s.get_name() << "]" << endl;

      /* Cycle through every param in 's' */
      for (size_t j=0; j<s.size(); j++) {
        const Param& p = s[j];
        
        /* Print based on param type_id, you can use is_<supported_type>() to have a bool response on that type */
        switch (p.type_id()) {
          case types::BOOL:   cout << "  " << p.get_name() << " = " << p.to_bool()   << endl; break;
          case types::INT:    cout << "  " << p.get_name() << " = " << p.to_int()    << endl; break;
          case types::FLOAT:  cout << "  " << p.get_name() << " = " << p.to_float()  << endl; break;
          case types::STRING: cout << "  " << p.get_name() << " = " << p.to_string() << endl; break;
          /* Note that the parsed string loses the quotation marks at the beginning/end */

          default: break;
        }
      }
    }

  }

  catch (toml::invalid_type_cast itc) {
    /* An exception thrown when there was an invalid cast from a Param through the apposite function 'to_<supported_type>()' */
    /* For example you cannot cast a STRING Param to a double value */
    cout << itc.reason << endl;
  }

  catch (toml::name_error ne) {
    /* An exception thrown at parse time, when a name of a param or of a section contains illegal characters */
    cout << ne.reason << endl;
    cout << "Details: {'" << ne.name << "' - caused the exception throw}" << endl;
  }

  catch (...) {
    /* To catch anything else */
    cout << "Uncaught error occurred..." << endl;
  }

  return 0;
}
