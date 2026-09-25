#include <iostream>
#include <string>
#pragma once

namespace QuackStart {

class QuackBootArgs {
public:
  QuackBootArgs(const QuackBootArgs &) = delete;
  QuackBootArgs &operator=(const QuackBootArgs &) = delete;

  static const QuackBootArgs &get(char **argv = nullptr) {
    static QuackBootArgs bootargs{argv};
    return bootargs;
  }

  const int &get_export_migrate_memfd() const { return export_migrate_memfd; }

private:
  const int export_migrate_memfd;

  explicit QuackBootArgs(char **argv)
      : export_migrate_memfd{
            std::move(std::stoi(argv[_export_migrate_memfd_idx]))} {}

  static constexpr int _export_migrate_memfd_idx = 1;
};

}; // namespace QuackStart
