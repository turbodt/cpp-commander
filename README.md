# Cpp Commander

The purpose of this library is to provide an easy way to handle CLI commands.

An example of use can be found [here](https://github.com/turbodt/cpp-commander-example-pizza).

### Integration

This library can be easy integrated using cmake:

```
include(FetchContent)

FetchContent_Declare(commander
  GIT_REPOSITORY https://github.com/turbodt/cpp-commander.git
  GIT_TAG main
)
FetchContent_MakeAvailable(commander)

[...]

add_executable(${PROJECT_NAME}
  ...
)

[...]

target_link_libraries(${PROJECT_NAME} PRIVATE Commander)
```

### Usage

For more detailed usage, look at the [example](https://github.com/turbodt/cpp-commander-example-pizza).

The library exposes a class `Command` from which your commands should extend.
Is recommended to create an intermediate custom base command so all your commands can extend to.
As example:


```cpp
#include "cpp-commander.h"

class BaseCommand : public Commander::Command {
public:
  using Commander::Command::Command;

  void initialize() {
    this->add("-h, --help", "Shows help.");
  }

protected:

  int execute_wrapper(Commander::ArgumentList *list) const {
    if (this->get_option("--help")->has_appeared()) {
      std::cout << this->get_help() << std::endl;
      return 0;
    }
    return this->Command::execute_wrapper(list);
  }
};
```


#### Basic command

To create a basic command, just extend from `Commander::Command` (or, as mentioned previously, from `BaseCommand`):

```cpp
class SimpleCommand : public BaseCommand {
public:
  using BaseCommand::BaseCommand;

  void initialize() {
    this->BaseCommand::initialize();
    this->set_description("This the definition of the command.")
        ->add("-m, --message <message>", "This is an option.")
        ->add("--debug", "This is an option without parameter.")
        ->add("<file>", "This is a mandatory positional argument.")
        ->add("[size]", "This is an optional argument."
                        " Should be added after all mandatory arguments.");
  }

  int execute() const {
    auto file = this->get_positional("file")->get_value();
    auto also_file = this->get_positional(0)->get_value();

    auto size = "xl";
    if (this->get_positional("size")->has_value()) {
      size = this->get_positional("size")->get_value();
    }

    if (this->get_option("--message")->has_appeared()) {
      auto message = this->get_option("--message")->get_value();
      // Do whatever
    }

    // Do whatever
    return 0;
  };
};
```

#### Composed command

Also is posible to group several subcommands into a command:

```cpp
class MainCommand : public BaseCommand {
public:
  using BaseCommand::BaseCommand;

  void initialize() {
    this->set_description("This is a command composed of several subcommands.")
        ->add("-l,--location <location>", "An example parameter")
        ->add_subcommand("subcommand1", new SubCommand1());
        ->add_subcommand("subcommand2", new SubCommand2());
  }
};
```

#### Execute the commands

Previous subsections explained how to actually _define_ the commands. The way of
run the commands is:

```cpp
int main(int argc, char *argv[]) {
  auto command = MainCommand();
  return command.from_main(argc, argv);
}
```

or

```cpp
command.from_line("myprogram -h");
```

### Other

I'm not an expert on `C++`, so please if you have any suggestion of improvement or find
any pattern that can suppose a problem on common integrations, please open an issue
and we'll try to improve the code.
Thanks.
