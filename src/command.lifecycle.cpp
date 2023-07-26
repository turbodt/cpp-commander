#include "./command.h"

namespace Commander {

// Creation
Command *Command::create_wrapper() {
    this->subcommands = new std::map<std::string const, Command *>();
    this->positional_arguments = new std::vector<PositionalArgument>();
    this->create();
    return this;
};

void Command::create() {}

// Initialization
Command *Command::initialize_wrapper() {
    if (!this->has_been_initializated) {
        this->has_been_initializated = true;
        this->initialize();
    }
    std::for_each(
        this->subcommands->begin(),
        this->subcommands->end(),
        [&](auto it) {
            it.second->initialize_wrapper();
        }
    );
    return this;
}

void Command::initialize() {}

// Parsing arguments
Command *Command::parse_arguments(ArgumentList *list) {
    this->label = list->get_current();
    list->next();

    bool exit = false;
    int positional_count = 0;
    while (!list->is_finished() && !exit) {
        auto argument = list->get_current();

        if (argument[0] == '-') {
            this->parse_option(list);
        } else if (positional_count < this->positional_arguments->size()) {
            // positional argument
            this->positional_arguments
                ->at(positional_count)
                .set_value(argument);

            this->parse_positional(positional_count, list);
            positional_count++;
        } else {
            // finish parsing
            exit = true;
        }
    }

    if (
        positional_count < this->positional_arguments->size()
        && !this->positional_arguments->at(positional_count).is_optional()
    ) {
        throw InvalidPositionalArgumentsNumberException(
            "Parsed less (" + std::to_string(positional_count)
            + ") positional arguments than expected."
        );
    }
    return this;
}

// Execution
int Command::execute_wrapper(ArgumentList *list) const {
    if (this->subcommands->size()) {
        return this->execute_subcommand(list);
    }
    return this->execute();
}

int Command::execute() const {
    std::cout << this->get_help() << std::endl;
    return 0;
}

// Combinations
int Command::parse_arguments_and_execute(ArgumentList *list) {
    try {
        this->parse_arguments(list);
    } catch (InvalidPositionalArgumentsNumberException &e) {
        std::cout << this->get_help() << std::endl;
        return 1;
    }
    return this->execute_wrapper(list);
}

} // namespace Commander
