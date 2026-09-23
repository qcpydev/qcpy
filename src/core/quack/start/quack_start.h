

#pragma once

namespace QuackStart {

class QuackBootArgs {
public:
  QuackBootArgs(int _export_migrate_memfd)
      : export_migrate_memfd{_export_migrate_memfd} {}

private:
  int export_migrate_memfd;
};

}; // namespace QuackStart
