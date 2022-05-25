#include "../include/command.hpp"

namespace Commander {

namespace get_help {
int get_max_size(std::vector<std::string> const &column) {
  int max_size = 0;
  std::for_each(column.cbegin(), column.cend(), [&](auto it) {
    if (it.size() > max_size) {
      max_size = it.size();
    }
  });
  return max_size;
}

int get_subcommand_label_max_width(
    std::map<std::string const, Command *> const *subcommands) {
  int max_size = 0;
  std::for_each(subcommands->begin(), subcommands->end(), [&](auto it) {
    if (max_size < it.first.size()) {
      max_size = it.first.size();
    }
  });
  return max_size;
}

int get_positional_argument_name_max_width(
    std::vector<PositionalArgument> const *positional_arguments) {
  int max_size = 0;
  std::for_each(positional_arguments->begin(), positional_arguments->end(),
                [&](auto positional_argument) {
                  if (max_size < positional_argument.get_name().size()) {
                    max_size = positional_argument.get_name().size();
                  }
                });
  return max_size;
}
}; // namespace get_help

std::string Command::get_help() const {
  std::stringstream ss;
  ss << "Usage:\n\n  " << this->label;
  std::for_each(this->positional_arguments->begin(),
                this->positional_arguments->end(),
                [&](auto it) { ss << " " << it.get_label(); });
  if (this->subcommands->size()) {
    ss << " SUBCOMMAND";
  }

  // description
  ss << "\n\n" << this->description;

  int max_entry_size = 0;

  if (this->positional_arguments->size()) {
    int name_max_size = get_help::get_positional_argument_name_max_width(
        this->positional_arguments);
  }
  if (this->subcommands->size()) {
  }

  // positional arguments
  if (this->positional_arguments->size()) {
    ss << "\n\nArguments:\n";
    int name_max_size = get_help::get_positional_argument_name_max_width(
        this->positional_arguments);
    std::for_each(this->positional_arguments->begin(),
                  this->positional_arguments->end(), [&](auto it) {
                    ss << "\n  " << std::setfill(' ')
                       << std::setw(name_max_size) << it.get_name() << "\t"
                       << it.get_description();
                  });
  }

  // sub commands
  if (this->subcommands->size()) {
    ss << "\n\nSub Commands:\n";
    int labels_max_size =
        get_help::get_subcommand_label_max_width(this->subcommands);
    std::for_each(
        this->subcommands->begin(), this->subcommands->end(), [&](auto it) {
          ss << "\n  " << std::setfill(' ') << std::setw(labels_max_size)
             << it.first << "\t" << it.second->get_description();
        });
    ss << "  ";
  }
  ss << std::flush;
  return ss.str();
};

} // namespace Commander
