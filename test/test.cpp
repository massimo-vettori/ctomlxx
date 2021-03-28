#include "../includes/ctomlxx.h"
#include <iostream>

using namespace toml;
using std::cout;
using std::endl;

int main() {
  try {
    Parser p;
    Root config;

    p.load("./test/test.toml"); /* Load the file in memory */
    p.parse(config);            /* Parse the loaded file */

    /*/////////////////////////////////////////////////////////////////////////////////////*/
    
    /* Now I want to access the first element of the inner list of the "array" param */
    /* I use referenced variables to avoid making copies of the values (they must be const though) */

    const Section& test = config["test"]; /* Same as calling config.get(...) */
    const Param& array  = test["array"];  /* Same as calling test.get(...) */

    int desired = array.to_vector()[2] /* Casts to vector of Params and then gets the inner list (index 2 on the example toml) */
                       .to_vector()[0] /* Casts to vector of Params and then gets the first element */
                       .to_int();      /* Casts the Param to primitive int */
    
    cout << "Desired value: " << desired << endl;
  } 
  
  catch (invalid_type_cast itc) {
    cout << itc.reason << endl;
  }

  catch (...) {
    cout << "Something went wrong..." << endl;
  }

  return 0;
}