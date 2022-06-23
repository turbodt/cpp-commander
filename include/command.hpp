#ifndef H_INCLUDE_COMMAND
#define H_INCLUDE_COMMAND

#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <vector>

#include "argument-list.hpp"
#include "descriptable.hpp"
#include "exceptions.hpp"
#include "option.hpp"
#include "positional-argument.hpp"

namespace Commander {

class Command : public Descriptable {
protected:
  // members
  std::map<std::string const, Command *> *subcommands;
  std::string label;
  std::vector<PositionalArgument> *positional_arguments;
  std::vector<Option> options;

  Command *parent = nullptr;

  // utils
  std::string get_help() const;

  // life cycle
  virtual Command *create_wrapper();
  virtual Command *initialize_wrapper();
  virtual int execute_wrapper(ArgumentList *) const;

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
  PositionalArgument const *get_global_positional(std::string const &) const;
  PositionalArgument const *get_global_positional(int) const;
  PositionalArgument const *get_positional(std::string const &) const;
  PositionalArgument const *get_positional(int) const;
  std::vector<PositionalArgument> get_positionals() const;
  bool has_option(const std::string &) const;
  Option const *get_option(const std::string &) const;
  Option const *get_option(int) const;
  Option *get_option(const std::string &);
  Option const *get_global_option(const std::string &) const;
  Option const *get_global_option(int) const;
  int get_global_option_size() const;
  std::vector<Option> const *get_options() const;
  std::vector<Option> *get_options();

  // setters
  Command *set_description(std::string const &description);

  // registration
  Command *add_positional(std::string const &);
  Command *add_positional(std::string const &, std::string const &);
  Command *add_positional(PositionalArgument &);
  Command *add_option(std::string const &);
  Command *add_option(std::string const &, std::string const &);
  Command *add_option(Option &);
  Command *add(std::string const &);
  Command *add(std::string const &, std::string const &);
  Command *add_subcommand(std::string const &, Command *);
  Command *add(std::string const &, Command *);

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
