#include <boot.h>

int main(int argc, char* argv[]) {
  assert(argc && argv[0]);

  boot_core(argc, argv);
}
