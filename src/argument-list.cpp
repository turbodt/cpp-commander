#include "../include/argument-list.hpp"

namespace Commander {

ArgumentList::ArgumentList(std::vector<std::string> const &list)
    : list(list), current(0){};

std::string const &ArgumentList::at(int index) const {
  return this->list.at(index);
}

std::string const &ArgumentList::get_current() const {
  return this->at(this->current);
}

int ArgumentList::size() const { return this->list.size(); }

ArgumentList *ArgumentList::next() {
  if (!this->is_finished()) {
    this->current++;
  }
  return this;
};

bool ArgumentList::is_finished() const {
  return this->current >= this->list.size();
}
} // namespace Commander
