#ifndef H_INCLUDE_POSITIONAL_ARGUMENT
#define H_INCLUDE_POSITIONAL_ARGUMENT

#include <iostream>

#include "descriptable.hpp"

namespace Commander {

class PositionalArgument : public Descriptable {

private:
  void on_creation(std::string const &);

protected:
  std::string name;
  std::string value;
  bool optional = false;

public:
  PositionalArgument(std::string const &);
  PositionalArgument(std::string const &, std::string const &);

  std::string get_name() const;
  std::string get_label() const;
  std::string get_value() const;
  bool is_optional() const;

  PositionalArgument *set_value(std::string const &value);
  PositionalArgument *set_optional(bool);
};
} // namespace Commander
#endif
