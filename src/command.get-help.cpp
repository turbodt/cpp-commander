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

int get_subcommands_label_max_width(
    std::map<std::string const, Command *> const *subcommands) {
  int max_size = 0;
  std::for_each(subcommands->begin(), subcommands->end(), [&](auto it) {
    if (max_size < it.first.size()) {
      max_size = it.first.size();
    }
  });
  return max_size;
}

int get_positional_arguments_name_max_width(
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

int get_options_name_max_width(std::vector<Option *> const *options) {
  int max_size = 0;
  std::for_each(options->cbegin(), options->cend(), [&](auto option) {
    if (max_size < option->get_label().size()) {
      max_size = option->get_label().size();
    }
  });
  return max_size;
}
}; // namespace get_help

std::string Command::get_help() const {
  std::stringstream ss;
  ss << "Usage:\n\n  " << this->label;

  if (this->get_options()->size()) {
    ss << " [OPTIONS]";
  }
  std::for_each(this->positional_arguments->begin(),
                this->positional_arguments->end(),
                [&](auto it) { ss << " " << it.get_label(); });
  if (this->subcommands->size()) {
    ss << " SUBCOMMAND";
  }

  // description
  if (this->get_description().size()) {
    ss << "\n\n" << this->get_description();
  }

  int max_entry_size = 0;

  if (this->positional_arguments->size()) {
    int name_max_size = get_help::get_positional_arguments_name_max_width(
        this->positional_arguments);
    max_entry_size = std::max(max_entry_size, name_max_size);
  }

  if (this->get_options()->size()) {
    int name_max_size =
        get_help::get_options_name_max_width(this->get_options());
    max_entry_size = std::max(max_entry_size, name_max_size);
  }

  if (this->subcommands->size()) {
    int subcommand_max_size =
        get_help::get_subcommands_label_max_width(this->subcommands);
    max_entry_size = std::max(max_entry_size, subcommand_max_size);
  }

  // positional arguments
  if (this->positional_arguments->size()) {
    ss << "\n\nArguments:\n";
    std::for_each(this->positional_arguments->begin(),
                  this->positional_arguments->end(),
                  [&](auto positional_argument) {
                    ss << "\n  " << std::setfill(' ')
                       << std::setw(max_entry_size)
                       << positional_argument.get_name();
                    std::stringstream description_ss(
                        positional_argument.get_description());
                    std::string line;
                    for (int i = 0; std::getline(description_ss, line); i++) {
                      if (i) {
                        ss << "\n  " << std::setfill(' ')
                           << std::setw(max_entry_size) << " ";
                      }
                      ss << "\t" << line;
                    }
                  });
  }

  // options
  if (this->get_options()->size()) {
    ss << "\n\nOptions:\n";
    std::for_each(this->get_options()->begin(), this->get_options()->end(),
                  [&](auto option) {
                    ss << "\n  " << std::setfill(' ')
                       << std::setw(max_entry_size) << option->get_label();

                    std::stringstream description_ss(option->get_description());
                    std::string line;
                    for (int i = 0; std::getline(description_ss, line); i++) {
                      if (i) {
                        ss << "\n  " << std::setfill(' ')
                           << std::setw(max_entry_size) << " ";
                      }
                      ss << "\t" << line;
                    }
                  });
  }

  // sub commands
  if (this->subcommands->size()) {
    ss << "\n\nSub Commands:\n";
    std::for_each(
        this->subcommands->begin(), this->subcommands->end(), [&](auto it) {
          ss << "\n  " << std::setfill(' ') << std::setw(max_entry_size)
             << it.first;
          std::stringstream description_ss(it.second->get_description());
          std::string line;
          for (int i = 0; std::getline(description_ss, line); i++) {
            if (i) {
              ss << "\n  " << std::setfill(' ') << std::setw(max_entry_size)
                 << " ";
            }
            ss << "\t" << line;
          }
        });
    ss << "  ";
  }
  ss << std::flush;
  return ss.str();
}; // namespace Commander

} // namespace Commander
