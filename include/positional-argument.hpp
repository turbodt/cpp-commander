#ifndef H_INCLUDE_POSITIONAL_ARGUMENT
#define H_INCLUDE_POSITIONAL_ARGUMENT

#include <iostream>
#include <regex>

#include "./descriptable.hpp"

namespace Commander {

class PositionalArgument : public Descriptable {
public:
    static const std::string label_regex_str;
    static const std::regex label_regex;

    PositionalArgument(std::string const &);
    PositionalArgument(std::string const &, std::string const &);

    std::string get_name() const;
    std::string get_label() const;
    std::string get_value() const;
    bool has_value() const;
    bool is_optional() const;

    PositionalArgument *set_value(std::string const &);
    PositionalArgument *set_optional(bool);

    static bool is_positional_argument_label(std::string const &);

protected:
    std::string name;
    std::string value;
    bool value_has_been_set = false;
    bool optional = false;

private:
    void on_creation(std::string const &);

};
} // namespace Commander
#endif
