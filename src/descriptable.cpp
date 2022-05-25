#include "../include/descriptable.hpp"

namespace Commander {
Descriptable::Descriptable(){};
Descriptable::Descriptable(std::string const &description)
    : description(description){};

Descriptable *Descriptable::set_description(std::string const &description) {
  this->description = description;
  return this;
};

std::string const &Descriptable::get_description() const {
  return this->description;
}
} // namespace Commander
