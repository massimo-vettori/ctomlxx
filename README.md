[![Build (C++)](https://github.com/massimo-vettori/ctomlxx/actions/workflows/cpp.yml/badge.svg)](https://github.com/massimo-vettori/ctomlxx/actions/workflows/cpp.yml)
# cTOMLxx ~ Simple TOML Parser
This `c++` TOML parser library can allow your code to load external `.toml` files containing configuration properties in order to change them without needing to recompile the entire project

### Features:
  - [x] `.toml` file parsing from path
  - [x] `boolean` type recognition
  - [x] `number` type recognition
  - [x] `floating point` type recognition
  - [x] `string` type recognition
  - [x] `inline array` type recognition
  - [x] `sections` recognition
  - [ ] `inline table` type recognition
  - [ ] `date/time` type recognition

### Install:
Inside the zipped folder in the lastest release you can find a `lib` folder and a `includes` folder.
Now you can install the library in 2 different ways (choose one, don't use both)

1. You can add the `libtoml.a` in the `lib` folder of the compiler and the `ctomlxx.h` header in the `includes` folder
Now you can simply refer to the header with the line `#include <ctomlxx.h>` in your source file and then you can add the flag `-ltoml` in the linking phase of the compilation
2. Otherwise you can add it directly into your project source: you just need to copy the folders from the zipped file to the directory where your project lies, then in the linking phase simply add `-L <path_to_the_library_directory> -ltoml`.
You may also want to add another flag to the compilation phase `-I <path_to_the_header_directory>` (if you want to include the header file as the example of the first method)


### Usage:
1. Now, when you included the header in your file, you can start to use the library...
First of all you need to setup a **Parser** object
    ```cpp
    #include <ctomlxx.h>
    
    using toml::Parser;
    
    int main() {
        Parser parser;
        return 0;
    }
    ```

2. After that we need to load a file to be able to parse it:
   _From now on we also may want to use a try/catch block, to catch any exception it can generate_
    ```cpp
    int main() {
        try {
            Parser parser;
            parser.load("path_to_the_toml_file");
        } 
        
        catch (toml::load_error le) {
            /* Handle the load error, if any */
            /* The load_error object has a string descriptor called 'reason' */
        }
        
        return 0;
    }
    ```

3. Now to the parse phase... We need to create a new Root object, to store the parsed values
    ```cpp
    using toml::Parser;
    using toml::Root;
    
    int main() {
        try {
            Parser parser;
            Root config;
            parser.load("path_to_the_toml_file");
            parser.parse(config);
        }
        
        catch (toml::load_error le) {
            /* Handle the load error, if any */
            /* The load_error object has a string descriptor called 'reason' */
        } 
        
        catch (toml::name_error ne) {
            /* Handle the name error, if any */
            /* As well as a descriptor (reason) for the fail reason it will store the 'bad' name (name) */
        } 
        
        catch (toml::uncaught_error ue) {
            /* Handle the uncaught error, if any */
            /* Generic error with a 'reason' parameter as a descriptor */
        }
        
        return 0;
    }
    ```
    
4. Now that the file was parsed correctly, we can access its internal sections and parameters
    * **NOTE** We can access to them with both operator [] and functional methods...
    * **NOTE** The `ctomlxx` library header will include also `string` and `vector`
    <br>
    
    ```cpp
    using toml::Parser;
    using toml::Root;
    using toml::Section;
    using toml::Param;
    
    int main() {
        try {
            Parser parser;
            Root config;
            parser.load("path_to_the_toml_file");
            parser.parse(config);
            
            Param one = config[0].at(0);
            Param two = config["test"]["second"];
            
            Section test = config["test"];
            Param three = test.get("third");
        }
        
        catch (toml::load_error le) {
            /* Handle the load error, if any */
            /* The load_error object has a string descriptor called 'reason' */
        } 
        
        catch (toml::name_error ne) {
            /* Handle the name error, if any */
            /* As well as a descriptor (reason) for the fail reason it will store the 'bad' name (name) */
        } 
        
        catch (toml::uncaught_error ue) {
            /* Handle the uncaught error, if any */
            /* Generic error with a 'reason' parameter as a descriptor */
        }
        
        return 0;
    }
    ```

-------
Now... lets understand what is going on...
 * First of all, the **Param one** gets the first *Sectio*n from the *Root* with the **operator []**, then gets the first *Param* of that *Section* with the functional method `.at(pos)`
 * Then for the second *Param*, we get the *Section* named "test" with the same **operator []**, but this time we use a **string** to get to the right *Section*, without having to search it by name manually (note that we can do it as well for *Param*s in *Section*s)
 * For the third param we first extract the *Section* (test) in a variable itself, then we extract the *Param* with the functional method `.get(string)`

### Questions and issues:
If you have any doubts, question, issues, or if you find any unresolved bugs, please feel free to create a new issue on this repository. I will be happy to try to answer your questions or solve any issues that may rise
