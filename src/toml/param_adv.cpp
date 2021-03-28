#include <ctomlxx.h>
#include <iostream>

using namespace toml;

///////////////////////////////////////////////////////////////////////////////////////////////////
size_t smart_find(char c, const string &s, size_t start=0, bool is_list=false) {
  if (start >= s.length()) return string::npos;
  if (c == '"')            return s.find_first_of(c, start);

  bool str     {false};
  int brackets {0};

  for (auto i=start; i<s.length(); i++) {
    if (is_list) {
      if (s.at(i) == '[') brackets++;
      if (s.at(i) == ']') brackets--;
    }

    if (not str and s.at(i) == c and not brackets) return i;
    if (s.at(i) == '"') str = not str;
  }

  return string::npos;
}

void smart_strip(string &s) {
  bool quote {false};

  const string pattern = " \t\r\n";
  
  for (auto i=0UL; i<s.length(); i++) {
    if      (s.at(i) == '"')                                               quote = !quote;
    else if (s.at(i) == '#' and not quote)                                 s.erase(i);
    else if (not quote and pattern.find_first_of(s.at(i)) != string::npos) s.erase(i--, 1);
  }
}

void truncate(string &s, bool keep_left=true, char divider='=') {
  size_t div = smart_find(divider, s);
  if (div != string::npos) {
    if (keep_left) s = s.substr(0, div);
    else           s = s.substr(div + 1);
  }
}

void smart_split(const string &s, vector<string> &out) {
  size_t start = 0;
  size_t comma = 0;
  do {
    string tmp;

    comma = smart_find(',', s, start, true);
    tmp   = s.substr(start, comma - start);

    if (not tmp.empty())
      out.push_back(tmp);
      
    start = comma+1;
  } while(comma != string::npos);
}

bool check_name(const string &s) {
  if (s.empty()) 
    throw toml::name_error{s, "Param names must have some characters"};

  if (s.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_") != string::npos)
    throw toml::name_error{s, "Param names must contain only alphanumeric characters or _ dividers"};

  if (string("0123456789").find_first_of(s.at(0)) != string::npos)
    throw toml::name_error{s, "Param names must not start with a numeric character, use _ instead"};

  return true;
}

types parse_type(const string& r) {
  if (r.empty())                                         return types::NIL;
  if (r == "true" or r == "false")                       return types::BOOL;
  if (r.at(0) == '"' and r.at(r.length()-1) == '"')      return types::STRING;
  if (r.at(0) == '[' and r.at(r.length()-1) == ']')      return types::VECTOR;
  if (r.find_first_not_of("0123456789") == string::npos) return types::INT;

  bool dot {false};
  for (char c: r) {
    if (string("0123456789").find_first_of(r) == string::npos) {
      if (c == '.' and !dot) dot = true;
      else return types::NIL;
    }
  }

  return types::FLOAT;
}
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
Bool* create_bool(const string& r) {
  return new Bool(r == "true");
}

Int* create_int(const string& r) {
  int val = atoi(r.c_str());
  return new Int(val);
}

Float* create_float(const string& r) {
  float val = atof(r.c_str());
  return new Float(val);
}

String* create_string(const string& r) {
  string val;
  if (r.length() <= 2) val = "";
  else val = r.substr(1, r.length() - 2);

  return new String(val);
}

Vector* create_vector(const string& r) {
  string         inner;
  vector<string> splitted;
  vector<Param>  params;
  
  if (r.length() <= 2) inner = "";
  else                 inner = r.substr(1, r.length() - 2);

  smart_split(inner, splitted);

  for (size_t i=0; i<splitted.size(); i++){
    params.push_back(Param("_"+std::to_string(i)+"="+splitted[i]));
  }

  return new Vector(params);
}
///////////////////////////////////////////////////////////////////////////////////////////////////

Param::Param(const string& raw) {
  string left  = raw;
  string right = raw;

  smart_strip(left);
  smart_strip(right);
  truncate(left, true);
  truncate(right, false);
  

  if ((left != right) and check_name(left)) {
    this->name = left;
    types type = parse_type(right);
    allocated  = true;
    
    switch (type) {
      case BOOL:   this->content = create_bool(right);   break;
      case INT:    this->content = create_int(right);    break;
      case FLOAT:  this->content = create_float(right);  break;
      case STRING: this->content = create_string(right); break;
      case VECTOR: this->content = create_vector(right); break;
      /* ... */
      default: allocated = false;
    }
  }
}
