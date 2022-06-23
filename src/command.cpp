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

Command *Command::add_option(Option &option) {
  this->get_options()->push_back(option);
  return this;
};

Command *Command::add_option(std::string const &label) {
  auto option = Option(label);
  return this->add_option(option);
};
Command *Command::add_option(std::string const &label,
                             std::string const &description) {
  auto option = Option(label, description);
  return this->add_option(option);
};

Command *Command::add(std::string const &label) {
  if (Option::is_option_label(label)) {
    return this->add_option(label);
  } else if (PositionalArgument::is_positional_argument_label(label)) {
    return this->add_positional(label);
  }
  throw InvalidLabelException(
      "Label `" + label +
      "` is neither a valid positional argument or option label.");
};
Command *Command::add(std::string const &label,
                      std::string const &description) {
  if (Option::is_option_label(label)) {
    return this->add_option(label, description);
  } else if (PositionalArgument::is_positional_argument_label(label)) {
    return this->add_positional(label, description);
  }
  throw InvalidLabelException(
      "Label `" + label +
      "` is neither a valid positional argument or option label.");
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
Command *Command::add(std::string const &name, Command *subcommand) {
  return this->add_subcommand(name, subcommand);
}

// utils
void Command::parse_option(ArgumentList *list) {
  if (this->has_option(list->get_current())) {
    this->get_option(list->get_current())->parse_argument_list(list);
  } else {
    list->next();
  }
}

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
