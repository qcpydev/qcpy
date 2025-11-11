#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#pragma once
#ifndef DOCK_H
#define DOCK_H


// turn importer structs here, create block directory which will own a block request
int dock_add();
void dock_run_boot(char* exec_name, char* args[]);
char* dock_init(char* args[]);


#endif // DOCK_H
