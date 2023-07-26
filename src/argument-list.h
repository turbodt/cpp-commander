#ifndef H_INCLUDE_ARGUMENT_LIST
#define H_INCLUDE_ARGUMENT_LIST

#include <iostream>
#include <vector>

namespace Commander {

class ArgumentList {
public:
    ArgumentList(std::vector<std::string> const &list);

    std::string const &at(int index) const;
    std::string const &get_current() const;
    bool is_finished() const;
    int size() const;

    ArgumentList *next();

protected:
    std::vector<std::string> list;
    int current = 0;
};

} // namespace Commander
#endif
