#include <block.h>

#pragma once
#ifndef EXPORTER_H
#define EXPORTER_H

/**
 * Port exporter manages, well, the exporting of data out of qcpy_core. This can
 * be either sending data to qcpy_connect (front end of qcpy) or sending qlog
 * entries to quack to process.
 */

void exporter_quack(export_t *export);
void exporter_init();

#endif
