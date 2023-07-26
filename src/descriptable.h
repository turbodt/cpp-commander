#ifndef H_INCLUDE_DESCRIPTABLE
#define H_INCLUDE_DESCRIPTABLE

#include <iostream>

namespace Commander {

class Descriptable {
public:
    Descriptable();
    Descriptable(std::string const &);
    virtual ~Descriptable() = default;

    std::string const &get_description() const;
    Descriptable *set_description(std::string const &);

protected:
    std::string description;
};

} // namespace Commander
#endif
