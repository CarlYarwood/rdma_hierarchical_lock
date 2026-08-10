#ifndef SPIN_LOCK_CLIENT_H
#define SPIN_LOCK_CLIENT_H
#include <time.h>
#include "spin.h"
#include "local_lock.h"

typedef struct {
	char * machine_lock_type;
	char * parent_address;
	long parent_port;
	uint64_t node_id;
	int critical_section;
	int noncritical_section;
	int num_aquire;
	union machine_lock machine_lock;
} spin_client_in;

typedef struct {
	struct rdma_cm_id* client_id;
	struct ibv_pd* pd;
    struct ibv_comp_channel* comp;
	struct ibv_cq* cq;
	struct ibv_mr* response_mr;
	struct ibv_mr* sync_mr;
	struct ibv_mr* server_metadata_mr;
	struct ibv_mr* client_metadata_mr;
	struct rdma_buffer_attr* server_metadata_attr;
	struct rdma_buffer_attr* client_metadata_attr;
} c_spin_ctx;

void* spin_client(void* in);
#endif