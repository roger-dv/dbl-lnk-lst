//
// Created by rogerv on 4/30/24.
//
#ifndef SOME_ELM_HPP
#define SOME_ELM_HPP

#include <string>

static inline char *rand_string(char *const str, size_t size) noexcept {
  static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  if (size > 0) {
    for (size_t n = 0; n < size; n++) {
      int key = rand() % (int) (sizeof charset - 1);
      str[n] = charset[key];
    }
    str[size] = '\0';
  }
  return str;
}

struct some_elm {
  static inline constinit int count = 0;
  static inline constinit bool prnt = true;
  std::string s{};
  // Overloading the equality operator (==)
  bool operator==(const some_elm &oth) const { return s == oth.s; }
  // Overloading the inequality operator (!=) using the default implementation
  bool operator!=(const some_elm& oth) const = default;
  some_elm() {
    const int nbr = std::max(rand() % (48 - 1), 5);
    s.resize(nbr);
    rand_string(s.data(), nbr);
  }
  some_elm(const some_elm &elm) noexcept : s{elm.s} {}
  some_elm(some_elm &&elm) noexcept {
    s = std::move(elm.s);
  }
  some_elm& operator=(const some_elm &elm) noexcept {
    s = elm.s;
    return *this;
  }
  some_elm& operator=(some_elm &&elm) noexcept {
    s = std::move(elm.s);
    return *this;
  }
  ~some_elm() {
    count -= 1;
    if (prnt && not s.empty())
      puts(s.c_str());
  }
};

#endif //SOME_ELM_HPP