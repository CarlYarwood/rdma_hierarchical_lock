#ifndef MCS_LOCK_CLIENT_H
#define MCS_LOCK_CLIENT_H
#include <time.h>
#include "mcs.h"
#include "local_lock.h"

#define NEXT (0)
#define NOTIFY (1)

#define SERVER (0)

typedef struct {
	uint64_t node_id;
	int critical_section;
	int noncritical_section;
	int num_aquire;
    char * machine_lock_type;
    char * parent_address;
    long parent_port;
    char ** peer_addresses;
    long * peer_ports;
    int num_peers;
    union machine_lock machine_lock;
} mcs_client_in;

typedef struct {
    uint64_t* node_id;
    struct ibv_pd* pd;
    struct ibv_comp_channel* comp;
    struct ibv_cq* cq;
    struct ibv_mr* buffer_mr;
    struct ibv_mr* metadata_mr;
    struct ibv_mr* server_metadata_mr;
    struct ibv_mr* client_metadata_mr;
    struct rdma_buffer_attr* server_metadata_attr;
    struct rdma_buffer_attr* client_metadata_attr;
} c_mcs_ctx;

void* mcs_client(void *in);
#endif