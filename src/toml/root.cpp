#include <ctomlxx.h>

using toml::Root;
using toml::Section;
using toml::Param;

void Root::append(const Section& s) {
  this->sections.push_back(s);
}

bool Root::has(const string& name) const {
  for (const Section& s: this->sections) {
    if (s.get_name() == name) return true;
  }

  return false;
}

const Section& Root::get(const string& name) const {
  for (const Section& s: this->sections) {
    if (s.get_name() == name) return s;
  }

  throw search_error{"Could not find a toml::Section with name '"+name+"'"};
}

const Section& Root::at(size_t pos) const {
  return this->sections.at(pos);
}


const Section& Root::operator[](const string& n) const {
  return this->get(n);
}

const Section& Root::operator[](size_t pos) const {
  return this->at(pos);
}


size_t Root::size() const {
  return this->sections.size();
}

bool Root::empty() const {
  return this->sections.empty();
}
