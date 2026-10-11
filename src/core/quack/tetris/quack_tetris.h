#include <block.h>
#include <qlog.h>
#include <quack_start.h>
#include <shared_mutex>
#include <thread>
#include <vector>

namespace QuackTetris {
typedef std::pair<QuackStart::QuackUnitaryMatrix,
                  QuackStart::QuackUnitaryMatrix>
    tetris_pair_t;

class QuackImporterTetris {
public:
  QuackImporterTetris(const QuackImporterTetris &) = delete;
  QuackImporterTetris operator=(const QuackImporterTetris &) = delete;

  QuackImporterTetris() : tetrify_matrix(QUBIT_MAX_SIZE), lock{} {};

  static const QuackImporterTetris &get() {
    static QuackImporterTetris obj;
    return obj;
  }

  static const void insert_entry(qlog_entry_t &qlog_entry, qubit_t qubit);

  mutable std::shared_mutex lock;

private:
  ~QuackImporterTetris() = default;
  std::vector<tetris_pair_t> tetrify_matrix;
  std::thread send_off_thread; // writer thread to send off to hive
};

}; // namespace QuackTetris
