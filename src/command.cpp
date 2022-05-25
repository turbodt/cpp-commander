#include "../include/command.hpp"

namespace Commander {
Command::Command() : Descriptable() { this->create_wrapper(); };
Command::Command(std::string const &description) : Descriptable(description) {
  this->create_wrapper();
};
Command::~Command() {
  std::for_each(this->subcommands->begin(), this->subcommands->end(),
                [&](auto it) { delete it.second; });
  delete this->subcommands;
  delete this->positional_arguments;
};

// getters
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

PositionalArgument const *Command::get_positional(int i) const {
  return &(this->positional_arguments->at(i));
}

std::vector<PositionalArgument> Command::get_positionals() const {
  auto positional_arguments = std::vector(this->positional_arguments->begin(),
                                          this->positional_arguments->end());
  return positional_arguments;
};

// setters
Command *Command::set_description(std::string const &description) {
  this->Descriptable::set_description(description);
  return this;
};

// registration
Command *Command::add_positional(PositionalArgument &positional_argument) {
  if (!positional_argument.is_optional() &&
      this->positional_arguments->size() &&
      this->positional_arguments->back().is_optional()) {
    throw InvalidAdditionException(
        "To add a positional argument after an optional positional argument is "
        "not allowed. (Invalid command label: `" +
        positional_argument.get_label() + "`)");
  } else if (positional_argument.is_optional() && this->subcommands->size()) {
    throw InvalidAdditionException(
        "To add an optional positional argument and a subcommand is "
        "not allowed.");
  }
  this->positional_arguments->push_back(positional_argument);
  return this;
};

Command *Command::add_positional(std::string const &label) {
  auto positional_argument = PositionalArgument(label);
  return this->add_positional(positional_argument);
};
Command *Command::add_positional(std::string const &label,
                                 std::string const &description) {
  auto positional_argument = PositionalArgument(label, description);
  return this->add_positional(positional_argument);
};

Command *Command::add_subcommand(std::string const &name, Command *subcommand) {
  if (this->positional_arguments->size() &&
      this->positional_arguments->back().is_optional()) {
    throw InvalidAdditionException(
        "To add a subcommand and an optional positional argument is "
        "not allowed.");
  }
  subcommand->parent = this;
  this->subcommands->insert(
      std::pair<std::string const, Command *>(name, subcommand));
  return this;
};

// utils
void Command::parse_option(ArgumentList *list) { list->next(); }

void Command::parse_positional(int index, ArgumentList *list) { list->next(); }

int Command::execute_subcommand(ArgumentList *list) const {

  if (list->is_finished()) {
    // do nothing
    std::cout << this->get_help() << std::endl;
    return 1;
  }

  auto argument = list->get_current();

  if (!this->subcommands->count(argument)) {
    std::cout << this->get_help() << std::endl;
    return 1;
  }

  auto subcommand = this->subcommands->at(argument);
  return subcommand->parse_arguments_and_execute(list);
};

int Command::from_main(int argc, char *argv[]) {
  auto arguments = std::vector<std::string>(argc);
  for (int i = 0; i < argc; i++) {
    arguments.at(i) = std::string(argv[i]);
  }
  this->initialize_wrapper();
  auto argument_list = new ArgumentList(arguments);
  int error = this->parse_arguments_and_execute(argument_list);
  delete argument_list;
  return error;
}

} // namespace Commander
