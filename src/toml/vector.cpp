#include <ctomlxx.h>

using toml::Generic;
using toml::Bool;
using toml::Int;
using toml::Float;
using toml::String;
using toml::Vector;
using toml::Param;

using toml::search_error;
using toml::name_error;
using toml::uncaught_error;

Vector::Vector(const vector<Param>& values) {
  for (Param v: values) {
    content.push_back(v);
  }
}

bool Vector::is_vector() const { return true; }

const vector<Param>& Vector::to_vector() const {
  return this->content;
}

const Param& Vector::operator[](size_t pos) const {
  return content.at(pos);
}

const Param& Vector::at(size_t pos) const {
  return content.at(pos);
}