#ifndef H_INCLUDE_OPTION
#define H_INCLUDE_OPTION

#include <iostream>
#include <regex>
#include <sstream>
#include <vector>

#include "argument-list.hpp"
#include "descriptable.hpp"
#include "exceptions.hpp"
#include "positional-argument.hpp"

namespace Commander {

class Option : public Descriptable {

private:
  static const std::regex name_regex;
  static const std::regex args_regex;

  void on_creation(std::string const &);
  void parse_label(std::string);

protected:
  std::vector<std::string> names;
  std::vector<PositionalArgument> values;
  bool has_been_parsed = false;

public:
  Option(std::string const &);
  Option(std::string const &, std::string const &);

  std::vector<std::string> *get_names();
  std::vector<std::string> const *get_names() const;
  std::vector<PositionalArgument> *get_values();
  std::vector<PositionalArgument> const *get_values() const;
  PositionalArgument const *get_value(int i) const;
  PositionalArgument *get_value(int i);
  bool has_name(std::string const &) const;
  std::string get_label() const;
  const bool has_appeared() const;

  Option *parse_argument_list(ArgumentList *);

  static bool is_option_label(std::string const &);
};
} // namespace Commander
#endif
