#include "../include/positional-argument.hpp"

namespace Commander {
PositionalArgument::PositionalArgument(std::string const &label)
    : Descriptable() {
  this->on_creation(label);
}
PositionalArgument::PositionalArgument(std::string const &label,
                                       std::string const &message)
    : Descriptable(message) {
  this->on_creation(label);
}

void PositionalArgument::on_creation(std::string const &label) {
  std::string parsed_label = label;
  if (parsed_label.at(0) == '[' && parsed_label.back() == ']') {
    parsed_label =
        std::string(parsed_label.begin() + 1, parsed_label.end() - 1);
    this->set_optional(true);
  } else if (parsed_label.at(0) == '<' && parsed_label.back() == '>') {
    parsed_label =
        std::string(parsed_label.begin() + 1, parsed_label.end() - 1);
    this->set_optional(false);
  }
  this->name = parsed_label;
}

std::string PositionalArgument::get_name() const { return this->name; }
std::string PositionalArgument::get_label() const {
  if (this->is_optional()) {
    return "[" + this->name + "]";
  }
  return "<" + this->name + ">";
}
std::string PositionalArgument::get_value() const { return this->value; }

bool PositionalArgument::is_optional() const { return this->optional; };

PositionalArgument *PositionalArgument::set_value(std::string const &value) {
  this->value = value;
  return this;
}
PositionalArgument *PositionalArgument::set_optional(bool value) {
  this->optional = value;
  return this;
}
} // namespace Commander
