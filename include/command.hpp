#ifndef H_INCLUDE_COMMAND
#define H_INCLUDE_COMMAND

#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "argument-list.hpp"
#include "descriptable.hpp"
#include "exceptions.hpp"
#include "positional-argument.hpp"

namespace Commander {

class Command : public Descriptable {
protected:
  // members
  std::map<std::string const, Command *> *subcommands;
  std::string label;
  std::vector<PositionalArgument> *positional_arguments;

  Command *parent = nullptr;

  // utils
  std::string get_help() const;

  // life cycle
  Command *create_wrapper();
  Command *initialize_wrapper();
  int execute_wrapper(ArgumentList *) const;

  int parse_arguments_and_execute(ArgumentList *);

public:
  Command();
  Command(std::string const &description);
  virtual ~Command();

  // getters
  bool has_parent() const;
  Command *get_parent();
  Command const *get_parent() const;
  int get_global_positionals_size() const;
  int get_positionals_size() const;
  PositionalArgument const *get_global_positional(int) const;
  PositionalArgument const *get_positional(int) const;
  std::vector<PositionalArgument> get_global_positionals() const;
  std::vector<PositionalArgument> get_positionals() const;

  // setters
  Command *set_description(std::string const &description);

  // registration
  Command *add_positional(std::string const &);
  Command *add_positional(std::string const &, std::string const &);
  Command *add_positional(PositionalArgument &);
  Command *add_subcommand(std::string const &, Command *);

  // utils
  virtual void parse_option(ArgumentList *);
  virtual void parse_positional(int, ArgumentList *);
  int execute_subcommand(ArgumentList *) const;

  // life cycle
  virtual void create();
  virtual void initialize();
  Command *parse_arguments(ArgumentList *);
  virtual int execute() const;
  int from_main(int argc, char *argv[]);
};
} // namespace Commander
#endif
