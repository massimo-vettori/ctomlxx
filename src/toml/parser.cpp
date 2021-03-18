#include <ctomlxx.h>
#include <fstream>

using std::ifstream;

using toml::Parser;
using toml::Root;
using toml::Section;
using toml::Param;
using toml::load_error;

///////////////////////////////////////////////////////////////////////////////////////////////////
bool is_section_name(const string& s) {
  string pattern = " \t";

  for (char c: s) {
    if (pattern.find_first_of(c) == string::npos) {
      if (c == '[') return true;
      else return false;
    }
  }

  return false;
}
///////////////////////////////////////////////////////////////////////////////////////////////////

void Parser::load(const string& path) {
  ifstream file(path);
  
  if (file.is_open()) {
    string line;
    this->flush();
    while (getline(file, line)) { lines.push_back(line); }
    file.close();
  }

  else {
    throw load_error{"Could not open file: "+path};
  }
}

void Parser::flush() {
  while (not this->lines.empty()) {
    this->lines.pop_back();
  }
}

void Parser::parse(Root& root) const {
  Section temp("[]");
  
  for (const string& s: this->lines) {
    if (is_section_name(s)) {
      if (not temp.undefined()) root.append(temp);
      temp = Section(s);                 
    } 
    
    else {
      if (temp.undefined()) temp = Section("[undefined]");
      temp.append(s);
    }
  }

  if (not temp.undefined()) root.append(temp);
}
