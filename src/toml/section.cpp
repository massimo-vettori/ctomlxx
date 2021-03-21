#include <ctomlxx.h>

using toml::Section;
using toml::Param;
using toml::uncaught_error;
using toml::name_error;
using toml::search_error;

///////////////////////////////////////////////////////////////////////////////////////////////////
void check_section_name(const string &s) {
  if (s.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890") != string::npos)
    throw toml::name_error{s, "Section names must contain only alphanumeric characters"};

  if (string("0123456789").find_first_of(s.at(0)) != string::npos)
    throw toml::name_error{s, "Section names must not start with a numeric character"};
}
///////////////////////////////////////////////////////////////////////////////////////////////////

Section::Section(const string& raw) {
  size_t lb = raw.find_first_of('[');
  size_t rb = raw.find_first_of(']');

  if (lb == string::npos) throw uncaught_error{"Missing left bracket from section name generator string"                };
  if (rb == string::npos) throw uncaught_error{"Missing right bracket from section name generator string"               };
  if (rb < lb)            throw uncaught_error{"Left and right brackets from section name generator string are inverted"};
  if ((rb - lb) == 1)     return;

  this->name = raw.substr( lb+1, rb-lb-1 );
  check_section_name(this->name);
}

bool Section::has(const string& n) const {
  for (const Param& p: this->params) {
    if (p.get_name() == n) return true;
  }

  return false;
}

const Param& Section::get(const string& n) const {
  for (const Param& p: this->params) {
    if (p.get_name() == n) return p;
  }

  throw search_error{"Could not find a toml::Param with name '"+n+"' within section '"+this->name+"'"};
}

const Param& Section::at(size_t pos) const {
  return this->params.at(pos);
}

const string& Section::get_name() const {
  return this->name;
}


const Param& Section::operator[](const string& n) const {
  return this->get(n);
}

const Param& Section::operator[](size_t pos) const {
  return this->at(pos);
}


void Section::append(const string& raw) {
  Param tmp(raw);
  if (tmp.type_id() != NIL)
    this->params.push_back(tmp);
}

bool Section::empty() const {
  return this->params.empty();
}

size_t Section::size() const {
  return this->params.size();
}

bool Section::undefined() const {
  return this->name == "";
}
