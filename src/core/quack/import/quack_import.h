#include <base_tools.h>
#include <exporter_migrate.h>
#include <quack_start.h>
#include <quack_tetris.h>
#include <sys/mman.h>
#include <thread>
#include <unistd.h>
#include <vector>

namespace QuackImport {
void quack_importer_worker_thread(size_t start_range);

class QuackImporter {
public:
  QuackImporter(const QuackImporter &) = delete;
  QuackImporter &operator=(const QuackImporter &) = delete;

  static const QuackImporter &get() {
    static QuackImporter instance{QuackStart::QuackBootArgs::get()};
    return instance;
  }

  exporter_migrate_t *get_exporter() const { return to_import_from; };

  friend void quack_importer_worker_thread(size_t start_range);

private:
  static constexpr size_t quack_import_thread_count = 4;
  static_assert(!(quack_import_thread_count - 1 & (quack_import_thread_count)));

  mutable exporter_migrate_t *to_import_from;
  std::vector<std::thread> threads;

  explicit QuackImporter(const QuackStart::QuackBootArgs &bootargs)
      : threads{quack_import_thread_count} {
    int exporter_shared_memfd = bootargs.get_export_migrate_memfd();

    to_import_from = (exporter_migrate_t *)mmap(
        NULL, sizeof(exporter_migrate_t), PROT_READ | PROT_WRITE, MAP_SHARED,
        exporter_shared_memfd, 0);

    assert(to_import_from != MAP_FAILED);

    for (auto i{0}; i < quack_import_thread_count; ++i) {
      threads.emplace_back(quack_importer_worker_thread, i);
    }
  };

  ~QuackImporter() {
    for (auto &thread : threads) {
      if (thread.joinable()) {
        thread.join();
      }
    }
  };
};
}; // namespace QuackImport
