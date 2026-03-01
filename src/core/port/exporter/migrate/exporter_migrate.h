#include <qlog.h>
#include <stdint.h>

#pragma once
#ifndef EXPORTER_MIGRATE_H
#define EXPORTER_MIGRATE_H

/*
 * "Pun intended" component of exporter to migrate work to send to quack,
 * regardless of cuda enabled or not. This shared memory space should not be
 * accessed by qcpy front end, which allows the connection between the qcpy_core
 * and quack_core/_gpu. We expect a stream of a qlog's information to be sent to
 * this shared memory space for quack to consume. This shared memory will be a
 * flat array (for now hopefully) that will be MAX_MIGRATE_SIZE *
 * MAX_QUEUE_SIZE * sizeof(qlog_entry_t) in size . Multiple entries can enter a
 * queue if they are able to merge together and suggests to quack that it should
 * do that (see quack_merge component). Once when all usable queues are full,
 * or if there are no more entries from the given qlog, signal to quack that
 * exporter_migrate space is ready to consume. quack should notify this
 * component when it is "hungry" for more qlog items.
 *
 * If a qlog has more than 16 qubits, then we will iterate to the next chunk,
 * where we will enter the same algorithm of chaining items. It is up to quack
 * to understand that this shared memory is not a 1:1 of a total quantum circuit
 * size, and that they must address doing calculations / organize entries it
 * consumes by itself.
 */

#define MAX_MIGRATE_SIZE 16
#define MAX_QUEUE_SIZE 16

#endif
