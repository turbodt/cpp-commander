#ifndef H_INCLUDE_DESCRIPTABLE
#define H_INCLUDE_DESCRIPTABLE

#include <iostream>

namespace Commander {

class Descriptable {
protected:
  std::string description;

public:
  Descriptable();
  Descriptable(std::string const &description);
  // setters
  Descriptable *set_description(std::string const &description);
  // getters
  std::string const &get_description() const;
};

} // namespace Commander
#endif
