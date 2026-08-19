#ifndef LOCAL_WORKER_H
#define LOCAL_WORKER_H
#include <time.h>
#include "local_lock.h"
#include "rdma_common.h"

typedef struct {
    uint64_t workder_id;
    volatile uint64_t * sync;
    union machine_lock machine_lock;
    char * machine_lock_type;
    int critical_section;
    int noncritical_section;
    int num_aquire;
}local_worker_in;
#endif