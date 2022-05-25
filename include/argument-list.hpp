#ifndef H_INCLUDE_ARGUMENT_LIST
#define H_INCLUDE_ARGUMENT_LIST

#include <iostream>
#include <vector>

namespace Commander {

class ArgumentList {
protected:
  std::vector<std::string> list;
  int current = 0;

public:
  ArgumentList(std::vector<std::string> const &list);

  std::string const &at(int index) const;
  std::string const &get_current() const;
  int size() const;

  ArgumentList *next();
  bool is_finished() const;
};
} // namespace Commander
#endif
