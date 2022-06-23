#include "../include/command.hpp"

namespace Commander {

bool Command::has_parent() const { return this->parent != nullptr; }
Command *Command::get_parent() { return this->parent; }
Command const *Command::get_parent() const { return this->parent; }

int Command::get_global_positionals_size() const {
  int size = this->get_positionals_size();
  if (this->has_parent()) {
    size += this->get_parent()->get_global_positionals_size();
  }
  return size;
};

int Command::get_positionals_size() const {
  return this->positional_arguments->size();
};

PositionalArgument const *Command::get_global_positional(int i) const {
  int offset = 0;
  if (this->has_parent()) {
    offset += this->get_parent()->positional_arguments->size();
    if (i < offset) {
      return this->get_parent()->get_global_positional(i);
    }
  }
  return this->get_positional(i - offset);
}

PositionalArgument const *
Command::get_global_positional(std::string const &name) const {
  for (int i = 0; i < this->get_global_positionals_size(); i++) {
    if (this->get_global_positional(i)->get_name() == name) {
      return this->get_global_positional(i);
    }
  }
  throw NotFoundException("Positional argument with name `" + name +
                          "` was not found.");
}

PositionalArgument const *Command::get_positional(int i) const {
  return &(this->positional_arguments->at(i));
}

PositionalArgument const *
Command::get_positional(std::string const &name) const {
  for (int i = 0; i < this->get_positionals_size(); i++) {
    if (this->get_positional(i)->get_name() == name) {
      return this->get_positional(i);
    }
  }
  throw NotFoundException("Positional argument with name `" + name +
                          "` was not found.");
}

std::vector<PositionalArgument> Command::get_positionals() const {
  auto positional_arguments = std::vector<PositionalArgument>(
      this->positional_arguments->begin(), this->positional_arguments->end());
  return positional_arguments;
};

bool Command::has_option(const std::string &name) const {
  for (auto &option : *this->get_options()) {
    if (option->has_name(name)) {
      return true;
    }
  }
  return false;
};

Option const *Command::get_option(const std::string &name) const {
  for (auto &option : *this->get_options()) {
    if (option->has_name(name)) {
      return option;
    }
  }
  throw NotFoundException("Option with name `" + name + "` was not found.");
};

Option const *Command::get_option(int index) const {
  return this->get_options()->at(index);
}

Option *Command::get_option(const std::string &name) {
  for (auto &option : *this->get_options()) {
    if (option->has_name(name)) {
      return option;
    }
  }
  throw NotFoundException("Option with name `" + name + "` was not found.");
};

Option const *Command::get_global_option(const std::string &name) const {
  if (this->has_parent()) {
    return this->get_parent()->get_global_option(name);
  }
  if (this->has_option(name)) {
    return this->get_option(name);
  }
  throw NotFoundException("Option with name `" + name +
                          "` was not globally found.");
};

Option const *Command::get_global_option(int index) const {
  int offset = 0;
  if (this->has_parent()) {
    offset += this->get_parent()->get_global_option_size();
    if (index < offset) {
      return this->get_parent()->get_global_option(index);
    }
  }
  return this->get_option(index - offset);
};

int Command::get_global_option_size() const {
  int size = this->get_options()->size();
  if (this->has_parent()) {
    size += this->get_parent()->get_global_option_size();
  }
  return size;
};

std::vector<Option *> const *Command::get_options() const {
  return &this->options;
}

std::vector<Option *> *Command::get_options() { return &this->options; }

} // namespace Commander
