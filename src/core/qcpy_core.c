#include <boot.h>

int main(int argc, char** argv)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    assert(argc && argv[0]);
    boot_core(argc, argv);
}
