#ifndef SPIN_CLUSTER_CLIENT_H
#define SPIN_CLUSTER_CLIENT_H
#include "spin_client.h"
#include "spin_server.h"

typedef struct {
	volatile uint64_t * sync;
	char * parent_address;
	long parent_port;
	uint64_t node_id;
    struct rdma_cm_id ** id_arr;
    uint64_t * lock;
    uint64_t * buffer;
    int handover;
} spin_cluster_client_in;

void* spin_cluster_client(void * in);
#endif