#include "./option.h"

namespace Commander {

std::string accepted_chars_for_labels = "A-Za-z0-9-";
std::string positional_argument_regex_str =
    "\\s*<?\\[?([A-Za-z0-9_:][-A-Za-z0-9_:]+)>?\\]?\\s*";

const std::regex Option::name_regex = std::regex(
    "^((\\s*(-[" + accepted_chars_for_labels + "]+)\\s*,?)+)( ("
    + positional_argument_regex_str + ")*)?\\s*$"
);

const std::regex Option::args_regex = std::regex(
    "^(\\s*(-[" + accepted_chars_for_labels + "]+)\\s*,?)+( ("
    + positional_argument_regex_str + ")*)?\\s*$"
);

Option::Option(std::string const &label) : Descriptable() {
    this->on_creation(label);
}
Option::Option(std::string const &label, std::string const &message)
    : Descriptable(message) {
    this->on_creation(label);
}

void Option::on_creation(std::string const &label) {
    this->parse_label(label);
}

bool Option::is_option_label(std::string const &label) {
    std::smatch sm;
    std::regex_match(label, sm, Option::name_regex);
    if (sm.size() < 2) {
        return false;
    }

    return true;
}

void Option::parse_label(std::string label) {
    if (!Option::is_option_label(label)) {
        throw InvalidLabelException(
            "Label `" + label + "` is not a valid option label."
        );
    }

    std::replace(label.begin(), label.end(), ',', ' ');
    std::smatch sm;

    // parse names
    std::regex_match(label, sm, Option::name_regex);
    std::string names_str = sm[1];
    std::stringstream name_ss(names_str);
    std::string name_str;

    this->get_names()->clear();
    while (name_ss >> name_str) {
        this->get_names()->push_back(name_str);
    }

    // parse values
    std::regex_match(label, sm, Option::args_regex);
    std::string args_str = sm[3];
    std::stringstream arg_ss(args_str);
    std::string arg_str;

    this->get_values()->clear();
    while (arg_ss >> arg_str) {
        auto positional_argument = PositionalArgument(arg_str);
        positional_argument.set_optional(false);
        this->get_values()->push_back(positional_argument);
    }
}

std::vector<std::string> const *Option::get_names() const {
    return &this->names;
}

std::vector<std::string> *Option::get_names() {
    return &this->names;
}

std::vector<PositionalArgument> const *Option::get_values() const {
    return &this->values;
}

std::vector<PositionalArgument> *Option::get_values() {
    return &this->values;
}

PositionalArgument const *Option::get_value(int i) const {
    return &(this->get_values()->at(i));
}

PositionalArgument *Option::get_value(int i) {
    return &(this->get_values()->at(i));
}

bool Option::has_name(std::string const &name) const {
    for (int i = 0; i < this->get_names()->size(); i++) {
        if (this->get_names()->at(i) == name) {
            return true;
        }
    }
    return false;
};

std::string Option::get_label() const {
    std::string label = "";

    for (int i = 0; i < this->get_names()->size(); i++) {
        if (i > 0) {
            label += ", ";
        }
        label += this->get_names()->at(i);
    }

    for (auto positional_argument : *(this->get_values())) {
        label += " " + positional_argument.get_label();
    }
    return label;
}

const bool Option::has_appeared() const {
    return this->has_been_parsed;
};

Option *Option::parse_argument_list(ArgumentList *list) {
    if (list->is_finished()) {
        throw OptionParsingException("Not enough arguments.");
    }
    if (!this->has_name(list->get_current())) {
        throw OptionParsingException("Names do not match.");
    }

    list->next();

    for (int i = 0; i < this->get_values()->size(); i++) {
        if (list->is_finished()) {
            throw OptionParsingException("Not enough arguments.");
        }
        this->get_value(i)->set_value(list->get_current());
        list->next();
    }

    this->has_been_parsed = true;

    return this;
};

} // namespace Commander
